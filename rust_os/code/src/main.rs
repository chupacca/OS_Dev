#![no_std] // don't link the Rust standard library
#![no_main] // disable all Rust-level entry points

use core::panic::PanicInfo;
mod vga_buffer; // Import a module `vga_buffer.rs` that handles the VGA buffer

/* 
 * This function is called when a panic happens
*/
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


// The static string we're going to print via VGA
//static HELLO: &[u8] = b"static hello";

/* 
* This function is the entry point, since the linker looks for a function
* named `_start` by default
*/
#[no_mangle] // This prevents the name of this function getting mangled
pub extern "C" fn _start() -> ! {

    // Test 4
    //  Note that we don’t have to import the macro in the main function, 
    //  because it already lives in the root namespace.
    println!("Hello World{}", "!");
    println!("Next String{}", "!");
    
    /*
    // Test 3
    use core::fmt::Write; // Need to import the fmt::Write trait in order to be able to use its functions
    vga_buffer::WRITER.lock().write_str("Hello again").unwrap();
    write!(vga_buffer::WRITER.lock(), ", some numbers: {} {}", 42, 1.337).unwrap();
    */

    // Test 2
    //vga_buffer::print_something();

    /*
    // Test 1
    let vga_buffer = 0xb8000 as *mut u8;

    for (i, &byte) in HELLO.iter().enumerate() {
        unsafe {
            *vga_buffer.offset(i as isize * 2) = byte; // char
            *vga_buffer.offset(i as isize * 2 + 1) = 0xd; // color
        }
    }
    */

    loop {} // we don't want to exit the start function
}




