///////////////////////////////////////////  ////////////////////////////////////////////
// STATIC VARIABLES ///////////////////////  ////////////////////////////////////////////

const BUFFER_HEIGHT: usize = 25;
const BUFFER_WIDTH: usize = 80;

///////////////////////////////////////////  ////////////////////////////////////////////
// ENUMS //////////////////////////////////  ////////////////////////////////////////////

#[allow(dead_code)] // silence the dead code warning cause we may not use some colors
#[derive(Debug, Clone, Copy, PartialEq, Eq)] // this way these methods can operate on these variables
#[repr(u8)] // keep in mind only the first 4 bits can be used for color in VGA
pub enum Color {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    Pink = 13,
    Yellow = 14,
    White = 15,
}

///////////////////////////////////////////  ////////////////////////////////////////////
// STRUCTS ////////////////////////////////  ////////////////////////////////////////////

#[derive(Debug, Clone, Copy, PartialEq, Eq)] // this way these methods can operate on these variables
#[repr(transparent)] // Makes sure this struct has the exact same data layout as a u8
struct ColorCode(u8);

impl ColorCode {
    fn new(foreground: Color, background: Color) -> ColorCode {
        ColorCode((background as u8) << 4 | (foreground as u8))
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)] // this way these methods can operate on these variables
#[repr(C)] // ordering in default structs in undefined rust so using this c attribute to gurantee field ordering
struct ScreenChar {
    ascii_character: u8,
    color_code: ColorCode,
}

use volatile::Volatile;

#[repr(transparent)] // Makes sure this struct has the exact same data layout as a u8
struct Buffer {
    chars: [[Volatile<ScreenChar>; BUFFER_WIDTH]; BUFFER_HEIGHT],
}

/*
 * We create a writer struct to help with writing to the screen.
 * Write to the last line and shift lines up when a line is full or on `\n`
 */
pub struct Writer {
    column_position: usize, // keeps tract of the current position in the last row
    color_code: ColorCode,  // current foreground and background colors
    buffer: &'static mut Buffer, // reference to the VGA buffer. the `'static` is for lifetime of a 
}

impl Writer {

    /*
     * Writes a byte of ASCII (unless i't a newline, then we call the `new_line()` method)
     */
    pub fn write_byte(&mut self, byte: u8) {

        // match is a switch statement in rust
        match byte {
            b'\n' => self.new_line(),
            byte => {
                if self.column_position >= BUFFER_WIDTH {
                    self.new_line();
                }

                let row = BUFFER_HEIGHT - 1;
                let col = self.column_position;

                let color_code = self.color_code;
                self.buffer.chars[row][col].write(ScreenChar {
                        ascii_character: byte,
                        color_code,
                    });
                self.column_position += 1;
            }
        }
    }

}

impl Writer {
    fn new_line(&mut self) {
        for row in 1..BUFFER_HEIGHT {
            for col in 0..BUFFER_WIDTH {
                let character = self.buffer.chars[row][col].read();
                self.buffer.chars[row - 1][col].write(character);
            }
        }
        self.clear_row(BUFFER_HEIGHT - 1);
        self.column_position = 0;
    }

    fn clear_row(&mut self, row: usize) {/* TODO */}
}

impl Writer {
    pub fn write_string(&mut self, s: &str) {
        for byte in s.bytes() {
            match byte {
                // printable ASCII byte or newline
                0x20..=0x7e | b'\n' => self.write_byte(byte),
                // not part of printable ASCII range
                _ => self.write_byte(0xfe),
            }

        }
    }
}


use spin::Mutex;
use lazy_static::lazy_static;

// The lazy_static! macro ensures that the WRITER variable is initialized lazily, 
//  which means that it is only initialized the first time it is accessed. 
//  This can be useful for minimizing the startup time of a program, because 
//  it allows you to delay the initialization of expensive resources until they are 
//  actually needed.
lazy_static! {
    /*
     We will use spinlock's mutex (spin::Mutex)

      Usually Mutex's can provide mutual exclusion by 
      blocking threads when the resource is already locked.
      But we haven't implemented threads or blocking support 
      yet in the bare metal kernel.

      We will use `spinlock` which, instead of blocking, the 
      threads simply try to lock it again and again in a tight loop, 
      thus burning CPU time until the mutex is free again.
      
      (there is a static mut in an unsafe declaration)
    */
    pub static ref WRITER: Mutex<Writer> = Mutex::new(
            Writer {
            column_position: 0,
            color_code: ColorCode::new(Color::Yellow, Color::Black),
            buffer: unsafe { &mut *(0xb8000 as *mut Buffer) }, // static mut is HIGHLY DISCOURAGED!!!
            // 0xb8000 : A memory location that is traditionally used for the text 
            //           buffer on computers with BIOS-based hardware.
        }
    );
}



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
use core::fmt;

impl fmt::Write for Writer {
    fn write_str(&mut self, s: &str) -> fmt::Result {
        self.write_string(s);
        Ok(())
    }
}

/* 
 Macros in Rust allow you to define code that generates other code at 
 compile time, which can be useful for generating repetitive or boilerplate code.
*/
#[macro_export] // Makes the macro available to the whole crate (not just the module it is defined in) and external crates
macro_rules! println {    // println macro has 2 rulses ...

    // Rule 1: Handle no paramters
    () => (print!("\n"));

    /* Rule 2: Handle parameters
     * `$` specify a variable that will be passed to the macro as an argument
     * `tt` is a placeholder that is used in macro definitions to match any token tree
     */
    ($($arg:tt)*) => (print!("{}\n", format_args!($($arg)*))); 
    // We do `print!` in this macro so we don't have to import print when importing println
}

#[macro_export]
macro_rules! print {
    ($($arg:tt)*) => ($crate::vga_buffer::_print(format_args!($($arg)*)));
}

#[doc(hidden)]
pub fn _print(args: fmt::Arguments) {
    use core::fmt::Write;
    WRITER.lock().write_fmt(args).unwrap();
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////



/*
 * This method is meant to test the VGA printing
 */
pub fn print_something() {
    use core::fmt::Write;
    let mut writer = Writer {
        column_position: 0,
        color_code: ColorCode::new(Color::Yellow, Color::Black),
        buffer: unsafe { &mut *(0xb8000 as *mut Buffer) },
    };

    writer.write_byte(b'H');
    writer.write_string("ello! ");
    write!(writer, "The numbers are {} and {}", 42, 1.0/3.0).unwrap();
}

