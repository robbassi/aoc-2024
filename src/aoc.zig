//! Util module for Advent of Code exercises.
const std = @import("std");
const File = std.fs.File;

/// A buffered, character reader. This is a low-effort implementation, there's
/// probably room for improvement.
pub const BufferedCharReaderBuffSize = 1024;
pub const BufferedCharReader = struct {
    const Self = @This();

    done: bool,
    index: usize,
    size: usize,
    file: File,
    buff: [BufferedCharReaderBuffSize]u8,

    pub fn init(file: File) Self {
        return .{
            .done = false,
            .index = 0,
            .size = 0,
            .file = file,
            .buff = undefined,
        };
    }

    // Try and fill the buffer. This will update self.size, with the number of
    // bytes read, 0 being EOF.
    fn fillBuff(self: *Self) File.ReadError!void {
        self.size = try self.file.read(&self.buff);
    }

    // Returns the next byte in the file. The result is an optional, where null
    // indicates EOF. If null is returned, then self done is also true.
    pub fn getChar(self: *Self) File.ReadError!?u8 {
        // Noop.
        if (self.done) return null;

        // Fill the buffer.
        if (self.index == self.size) {
            try self.fillBuff();
            self.index = 0;
            if (self.size == 0) {
                self.done = true;
                return null;
            }
        }

        // Return the next byte.
        const char = self.buff[self.index];
        self.index += 1;
        return char;
    }

    // Skips the next n bytes.
    pub fn skip(self: *Self, n: usize) File.ReadError!void {
        for (0..n) |_| {
            _ = try self.getChar();
        }
    }
};
