use std::{ffi::CStr, fmt, io};
use vaal_sys as ffi;

#[derive(Debug, Clone)]
pub enum Error {
    VAALError(&'static str),
    WrapperError(String),
    Null(),
    IoError(io::ErrorKind),
}

impl From<ffi::VAALError> for Error {
    fn from(value: ffi::VAALError) -> Self {
        let ret = unsafe { ffi::vaal_strerror(value) };
        if ret.is_null() {
            return Error::Null();
        }
        let desc = unsafe { CStr::from_ptr(ret) };
        match desc.to_str() {
            Ok(estr) => return Error::VAALError(estr),
            Err(_) => return Error::Null(),
        }
    }
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Error::VAALError(e) => return write!(f, "{}", e),
            Error::WrapperError(e) => return write!(f, "{}", e),
            Error::Null() => return write!(f, "null/unknown error message unavailable"),
            Error::IoError(kind) => {
                let e = std::io::Error::from(*kind);
                return write!(f, "{}", e);
            }
        }
    }
}

impl std::error::Error for Error {}
