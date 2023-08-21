use deepviewrt as dvrt;
use std::ffi::CStr;
use std::ffi::CString;
use std::io;
use vaal_sys as ffi;
pub mod error;
use error::Error;

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

    pub fn load_model_file(&self, filename: &str) -> Result<(), Error> {
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

    pub fn unload_model(&mut self) -> Result<(), Error> {
        let result = unsafe { ffi::vaal_unload_model(self.ptr) };

        if result != ffi::VAALError_VAAL_SUCCESS {
            return Err(Error::from(result));
        }
        return Ok(());
    }

    pub fn load_frame_dmabuf(
        &self,
        tensor: &dvrt::NNTensor,
        handle: i32,
        fourcc: u32,
        width: i32,
        height: i32,
        proc: u32,
    ) -> Result<(), Error> {
        let result = unsafe {
            ffi::vaal_load_frame_dmabuf(
                self.ptr,
                tensor.to_mut_ptr() as *mut ffi::NNTensor,
                handle,
                fourcc,
                width,
                height,
                std::ptr::null(),
                proc,
            )
        };
        if result != ffi::VAALError_VAAL_SUCCESS {
            return Err(Error::from(result));
        }
        return Ok(());
    }

    pub fn label(&self, index: i32) -> Result<&str, Error> {
        let result = unsafe { ffi::vaal_label(self.ptr, index as std::os::raw::c_int) };

        if result.is_null() {
            return Err(Error::WrapperError(
                "invalid label index".to_string(),
            ));
        }
        let cstr_label = unsafe { CStr::from_ptr(result) };
        let label = match cstr_label.to_str() {
            Ok(label) => label,
            Err(e) => return Err(Error::WrapperError(e.to_string())),
        };
        if label == "" {
            return Err(Error::WrapperError(
                "invalid label index".to_string(),
            ));
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
}

impl Drop for Context {
    fn drop(&mut self) {
        unsafe { ffi::vaal_context_release(self.ptr) };
    }
}
