use deepviewrt as dvrt;
use std::{
    ffi::{CStr, CString},
    io,
    path::Path,
    ptr,
};
use vaal_sys as ffi;
pub mod error;
use error::Error;
pub use ffi::VAALBox;

pub fn clock_now() -> i64 {
    return unsafe { ffi::vaal_clock_now() };
}

pub struct Context {
    ptr: *mut ffi::VAALContext,
}

unsafe impl Send for Context {}

impl Context {
    pub fn new(device: &str) -> Result<Self, Error> {
        let device_cstring = match CString::new(device.as_bytes()) {
            Ok(device_cstring) => device_cstring,
            Err(e) => return Err(Error::WrapperError(e.to_string())),
        };
        let ptr = unsafe { ffi::vaal_context_create(device_cstring.as_ptr()) };
        if ptr.is_null() {
            return Err(Error::IoError(io::Error::last_os_error().kind()));
        }

        return Ok(Context { ptr });
    }

    pub fn dvrt_context(&self) -> Result<dvrt::context::NNContext, Error> {
        let ret = unsafe { ffi::vaal_context_deepviewrt(self.ptr) };
        let context = unsafe { dvrt::context::NNContext::from_ptr(ret as _) };
        if context.is_err() {
            if let Err(dvrt::error::Error::WrapperError(string)) = context {
                return Err(Error::WrapperError(string));
            }
        }
        return Ok(context.unwrap());
    }

    pub fn load_model_file<P: AsRef<Path> + Into<Vec<u8>>>(
        &self,
        filename: P,
    ) -> Result<(), Error> {
        let c_str_filename = CString::new(filename).unwrap();
        let result = unsafe {
            ffi::vaal_load_model_file(
                self.ptr,
                c_str_filename.as_ptr() as *const std::os::raw::c_char,
            )
        };
        if result != ffi::VAALError_VAAL_SUCCESS {
            return Err(Error::from(result));
        }
        return Ok(());
    }

    pub fn boxes(&self, boxes: &mut Vec<VAALBox>, max_len: usize) -> Result<usize, Error> {
        let mut num_boxes: usize = 0;
        let ret = unsafe {
            ffi::vaal_boxes(
                self.ptr,
                boxes.as_mut_ptr(),
                max_len,
                &mut num_boxes as *mut usize,
            )
        };
        unsafe { boxes.set_len(num_boxes) }
        if ret != ffi::VAALError_VAAL_SUCCESS {
            return Err(Error::from(ret));
        }
        return Ok(num_boxes);
    }

    pub fn model(&self) -> Result<dvrt::model::Model, Error> {
        let ret = unsafe { ffi::vaal_context_model(self.ptr) };
        if ret.is_null() {
            let err = String::from("no model loaded");
            return Err(Error::WrapperError(err));
        }

        let ret = dvrt::model::Model::try_from_ptr(ret);
        if let Err(e) = ret {
            return Err(Error::WrapperError(e.to_string()));
        }
        return Ok(ret.unwrap());
    }

    pub fn unload_model(&mut self) -> Result<(), Error> {
        let result = unsafe { ffi::vaal_unload_model(self.ptr) };

        if result != ffi::VAALError_VAAL_SUCCESS {
            return Err(Error::from(result));
        }
        return Ok(());
    }

    pub fn load_frame_dmabuf(
        &self,
        tensor: Option<&dvrt::tensor::NNTensor>,
        handle: i32,
        fourcc: u32,
        width: i32,
        height: i32,
        roi: Option<&[i32; 4]>,
        proc: u32,
    ) -> Result<(), Error> {
        let roi_ = if roi.is_none() {
            std::ptr::null()
        } else {
            roi.unwrap().as_ptr()
        };
        let ptr;
        if tensor.is_some() {
            ptr = tensor.unwrap().to_mut_ptr() as *mut ffi::NNTensor;
        } else {
            ptr = ptr::null_mut();
        }
        let result = unsafe {
            ffi::vaal_load_frame_dmabuf(self.ptr, ptr, handle, fourcc, width, height, roi_, proc)
        };
        if result != ffi::VAALError_VAAL_SUCCESS {
            return Err(Error::from(result));
        }
        return Ok(());
    }

    pub fn run_model(&self) -> Result<(), Error> {
        let ret = unsafe { ffi::vaal_run_model(self.ptr) };
        if ret != 0 {
            return Err(Error::from(ret));
        }

        return Ok(());
    }

    pub fn output_tensor(&self, index: i32) -> Option<dvrt::tensor::NNTensor> {
        let ret = unsafe { ffi::vaal_output_tensor(self.ptr, index) };
        if ret.is_null() {
            return None;
        }

        let tensor = unsafe { dvrt::tensor::NNTensor::from_ptr(ret as _, false) };
        if tensor.is_err() {
            return None;
        }
        return Some(tensor.unwrap());
    }

    pub fn output_count(&self) -> Result<i32, Error> {
        let ret = unsafe { ffi::vaal_output_count(self.ptr) };
        if ret == 0 {
            return Err(Error::WrapperError(String::from(
                "context is invalid, has no model loaded, or the model does not identify any outputs",
            )));
        }

        return Ok(ret);
    }

    pub fn output_name(&self, index: i32) -> Option<&str> {
        let ret = unsafe { ffi::vaal_output_name(self.ptr, index) };
        if ret.is_null() {
            return None;
        }

        let cstr_name = unsafe { CStr::from_ptr(ret) };
        match cstr_name.to_str() {
            Ok(name) => return Some(name),
            Err(_) => return None,
        };
    }

    pub fn label(&self, index: i32) -> Result<&str, Error> {
        let result = unsafe { ffi::vaal_label(self.ptr, index as std::os::raw::c_int) };

        if result.is_null() {
            return Err(Error::WrapperError("invalid label index".to_string()));
        }
        let cstr_label = unsafe { CStr::from_ptr(result) };
        let label = match cstr_label.to_str() {
            Ok(label) => label,
            Err(e) => return Err(Error::WrapperError(e.to_string())),
        };
        if label == "" {
            return Err(Error::WrapperError("invalid label index".to_string()));
        }
        return Ok(label);
    }

    pub fn labels(&self) -> Vec<&str> {
        let mut labels = Vec::<&str>::new();
        let mut i = 0;
        loop {
            match self.label(i) {
                Ok(label) => labels.push(label),
                Err(_) => {
                    return labels;
                }
            };
            i += 1;
        }
    }

    pub fn parameter_gets() {}

    pub fn parameter_sets(&self, name: &str, value: &str) -> Result<(), Error> {
        let len = value.len();
        let name = match CString::new(name) {
            Ok(name) => name,
            Err(e) => return Err(Error::WrapperError(e.to_string())),
        };
        let value = match CString::new(value) {
            Ok(value) => value,
            Err(e) => return Err(Error::WrapperError(e.to_string())),
        };
        let ret =
            unsafe { ffi::vaal_parameter_sets(self.ptr, name.into_raw(), value.into_raw(), len) };
        if ret != ffi::VAALError_VAAL_SUCCESS {
            return Err(Error::from(ret));
        }
        return Ok(());
    }

    pub fn parameter_getf() {}

    pub fn parameter_setf(&self, name: &str, value: &[f32]) -> Result<(), Error> {
        let len = value.len();
        let name = match CString::new(name) {
            Ok(name) => name,
            Err(e) => return Err(Error::WrapperError(e.to_string())),
        };
        let ret =
            unsafe { ffi::vaal_parameter_setf(self.ptr, name.into_raw(), value.as_ptr(), len) };
        if ret != ffi::VAALError_VAAL_SUCCESS {
            return Err(Error::from(ret));
        }
        return Ok(());
    }

    pub fn parameter_geti() {}

    pub fn parameter_seti(&self, name: &str, value: &[i32]) -> Result<(), Error> {
        let len = value.len();
        let name = match CString::new(name) {
            Ok(name) => name,
            Err(e) => return Err(Error::WrapperError(e.to_string())),
        };
        let ret =
            unsafe { ffi::vaal_parameter_seti(self.ptr, name.into_raw(), value.as_ptr(), len) };
        if ret != ffi::VAALError_VAAL_SUCCESS {
            return Err(Error::from(ret));
        }
        return Ok(());
    }

    pub fn parameter_getu() {}

    pub fn parameter_setu(&self, name: &str, value: &[u32]) -> Result<(), Error> {
        let len = value.len();
        let name = match CString::new(name) {
            Ok(name) => name,
            Err(e) => return Err(Error::WrapperError(e.to_string())),
        };
        let ret =
            unsafe { ffi::vaal_parameter_setu(self.ptr, name.into_raw(), value.as_ptr(), len) };
        if ret != ffi::VAALError_VAAL_SUCCESS {
            return Err(Error::from(ret));
        }
        return Ok(());
    }
}

impl Drop for Context {
    fn drop(&mut self) {
        unsafe { ffi::vaal_context_release(self.ptr) };
    }
}
