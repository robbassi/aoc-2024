const std = @import("std");

pub fn processInput(file: std.fs.File, list_a: *std.ArrayList(i32), list_b: *std.ArrayList(i32)) void {
    var buf: [1024]u8 = undefined;
    const reader = file.reader();
    read: while (true) {
        const line = reader.readUntilDelimiterOrEof(&buf, '\n') catch unreachable orelse break :read;
        var it = std.mem.splitAny(u8, line, " ");
        const a = std.fmt.parseInt(i32, it.next().?, 10) catch unreachable;
        _ = it.next();
        _ = it.next();
        const b = std.fmt.parseInt(i32, it.next().?, 10) catch unreachable;
        list_a.append(a) catch unreachable;
        list_b.append(b) catch unreachable;
    }
}

pub fn main() !void {
    var general_purpose_allocator = std.heap.GeneralPurposeAllocator(.{}){};
    var list_a = try std.ArrayList(i32).initCapacity(general_purpose_allocator.allocator(), 1024);
    var list_b = try std.ArrayList(i32).initCapacity(general_purpose_allocator.allocator(), 1024);
    const stdin = std.io.getStdIn();

    processInput(stdin, &list_a, &list_b);
    const list_a_data = try list_a.toOwnedSlice();
    const list_b_data = try list_b.toOwnedSlice();
    std.mem.sort(i32, list_a_data, {}, std.sort.asc(i32));
    std.mem.sort(i32, list_b_data, {}, std.sort.asc(i32));

    std.debug.assert(list_a_data.len == list_b_data.len);

    var total_distance: i32 = 0;
    for (list_a_data, list_b_data) |a, b| {
        total_distance += if (a > b) a - b else b - a;
    }
    std.debug.print("{}\n", .{total_distance});
}
