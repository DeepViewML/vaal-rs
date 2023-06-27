#!/bin/sh

bindgen --allowlist-function 'vaal_.*' vaal.h > src/ffi.rs
