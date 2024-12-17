const std = @import("std");
const c = @cImport({
    @cInclude("stdio.h");
});
const File = std.fs.File;

pub fn processInput(file: File, list_a: *std.ArrayList(i32), list_b: *std.ArrayList(i32)) !void {
    var buf: [1024]u8 = undefined;
    const reader = file.reader();
    read: while (true) {
        const line = reader.readUntilDelimiter(&buf, '\n') catch break :read;
        var it = std.mem.splitAny(u8, line, " ");
        const a = std.fmt.parseInt(i32, it.next().?, 10) catch unreachable;
        _ = it.next();
        _ = it.next();
        const b = std.fmt.parseInt(i32, it.next().?, 10) catch unreachable;
        try list_a.append(a);
        try list_b.append(b);
    }
}

pub fn processInputWithScanf(list_a: *std.ArrayList(i32), list_b: *std.ArrayList(i32)) !void {
    var a: i32 = 0;
    var b: i32 = 0;
    while (c.scanf("%d   %d", &a, &b) == 2) {
        try list_a.append(a);
        try list_b.append(b);
    }
}

/// A buffered, character reader. This is a low-effort implementation, there's
/// probably room for improvement.
const BufferedCharReaderBuffSize = 1024;
const BufferedCharReader = struct {
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

    fn fillBuff(self: *Self) File.ReadError!void {
        self.size = try self.file.read(&self.buff);
    }

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

        // Return the next character.
        const char = self.buff[self.index];
        self.index += 1;
        return char;
    }

    pub fn skip(self: *Self, n: usize) File.ReadError!void {
        for (0..n) |_| {
            _ = try self.getChar();
        }
    }
};

/// Process input using the BufferedCharReader.
pub fn processInputWithCustomReader(file: std.fs.File, list_a: *std.ArrayList(i32), list_b: *std.ArrayList(i32)) !void {
    var reader = BufferedCharReader.init(file);
    while (!reader.done) {
        var a: i32 = 0;
        var b: i32 = 0;

        // Read a.
        while (try reader.getChar()) |char| {
            switch (char) {
                '0'...'9' => a = (char - 48) + a * 10,
                else => {
                    try list_a.append(a);
                    break;
                },
            }
        }

        // Skip spaces.
        try reader.skip(2);

        // Read b.
        while (try reader.getChar()) |char| {
            switch (char) {
                '0'...'9' => b = (char - 48) + b * 10,
                else => {
                    try list_b.append(b);
                    break;
                },
            }
        }
    }
}

pub fn printUsage(prog_name: [*:0]u8) void {
    std.debug.print("Usage: {s} [scanf|reader]\n", .{prog_name});
}

pub fn main() !void {
    var list_a = try std.ArrayList(i32).initCapacity(std.heap.page_allocator, 1024);
    var list_b = try std.ArrayList(i32).initCapacity(std.heap.page_allocator, 1024);
    const stdin = std.io.getStdIn();

    // Build the lists.
    switch (std.os.argv.len) {
        // Use a custom reader. As fast as the C version, but in pure Zig.
        1 => try processInputWithCustomReader(stdin, &list_a, &list_b),
        2 => {
            const mode_str = std.os.argv[1];
            const mode: []const u8 = std.mem.span(mode_str);
            if (std.mem.eql(u8, mode, "scanf")) {
                // Up to 4x faster than using the Reader approach, and matches
                // C performance. However, this uses scanf from libc, to do the
                // heavy lifting.
                try processInputWithScanf(&list_a, &list_b);
            } else if (std.mem.eql(u8, mode, "reader")) {
                // Uses std.io.Reader, and std.fmt.parseInt. This is how I
                // approached it initially, this seemed the most "correct".
                try processInput(stdin, &list_a, &list_b);
            } else {
                printUsage(std.os.argv[0]);
                return;
            }
        },
        else => {
            printUsage(std.os.argv[0]);
            return;
        },
    }
    std.debug.assert(list_a.items.len == list_b.items.len);

    // Sort the data.
    std.mem.sort(i32, list_a.items, {}, std.sort.asc(i32));
    std.mem.sort(i32, list_b.items, {}, std.sort.asc(i32));

    // Compute the distance.
    var total_distance: u32 = 0;
    for (list_a.items, list_b.items) |a, b| {
        total_distance += @abs(a - b);
    }

    // Compute the similarity score.
    var similaritry_score: i32 = 0;
    var last_a: i32 = -1;
    var freq: i32 = 0;
    var j: usize = 0;
    for (list_a.items) |a| {
        if (a != last_a) {
            freq = 0;
            for (list_b.items[j..]) |b| {
                if (b < a) {
                    continue;
                } else if (b == a) {
                    freq += 1;
                } else {
                    break;
                }
                j += 1;
            }
        }
        similaritry_score += a * freq;
        last_a = a;
    }

    // Output.
    std.debug.print("{}\n", .{total_distance});
    std.debug.print("{}\n", .{similaritry_score});
}
