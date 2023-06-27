use std::ffi::CString;
use std::error::Error;
use std::io; 
use vaal_sys as ffi;
use deepview_rt as dvrt;

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

		return Ok(Context {ptr});
	}

	pub fn load_frame_dmabuf(&self, tensor: &dvrt::NNTensor, handle: i32, fourcc: u32, width: i32, height: i32,  proc: u32) -> Result<(), ()>{

		let result = unsafe { ffi::vaal_load_frame_dmabuf(self.ptr, tensor.to_mut_ptr() as *mut ffi::NNTensor, handle, fourcc, width, height, std::ptr::null(), proc) };
		if result == ffi::VAALError_VAAL_SUCCESS {
			return Ok(());
		}
		return Err(());
	}
}
