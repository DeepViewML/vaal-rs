use deepviewrt as dvrt;
use std::{error::Error, ffi::CString, io};
use vaal_sys as ffi;

pub struct Context {
    ptr: *mut ffi::VAALContext,
}

impl Context {
    pub fn new(device: &str) -> Result<Self, Box<dyn Error>> {
        let device_cstring = CString::new(device.as_bytes())?;
        let ptr = unsafe { ffi::vaal_context_create(device_cstring.as_ptr()) };
        if ptr.is_null() {
            let err = io::Error::last_os_error();
            return Err(Box::new(err));
        }

        return Ok(Context { ptr });
    }

    pub fn load_frame_dmabuf(
        &self,
        tensor: &dvrt::NNTensor,
        handle: i32,
        fourcc: u32,
        width: i32,
        height: i32,
        roi: Option<&[i32; 4]>,
        proc: u32,
    ) -> Result<(), ()> {
        let roi_ = if roi.is_none() {
            std::ptr::null()
        } else {
            roi.unwrap().as_ptr()
        };
        let result = unsafe {
            ffi::vaal_load_frame_dmabuf(
                self.ptr,
                tensor.to_mut_ptr() as *mut ffi::NNTensor,
                handle,
                fourcc,
                width,
                height,
                roi_,
                proc,
            )
        };
        if result == ffi::VAALError_VAAL_SUCCESS {
            return Ok(());
        }
        return Err(());
    }
}
