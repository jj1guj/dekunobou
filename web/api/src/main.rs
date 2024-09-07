extern crate dekunobou_lib;
use std::ffi::CString;
use std::os::raw::c_char;

fn main() {
    unsafe {
        let c_string: CString = CString::new("0000000000000000000000000002100000012000000000000000000000000000").unwrap();
        let c_string_ptr: *const c_char = c_string.as_ptr();
        println!("{}", dekunobou_lib::dekunobou(c_string_ptr, false, 10, 15));
    }
}
