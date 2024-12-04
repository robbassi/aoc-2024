const std = @import("std");

pub fn processInput(file: std.fs.File, list_a: *std.ArrayList(i32), list_b: *std.ArrayList(i32)) void {
    var buf: [1024]u8 = undefined;
    const reader = file.reader();
    read: while (true) {
        const line = reader.readUntilDelimiter(&buf, '\n') catch break :read;
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
    var list_a = try std.ArrayList(i32).initCapacity(std.heap.page_allocator, 1024);
    var list_b = try std.ArrayList(i32).initCapacity(std.heap.page_allocator, 1024);
    const stdin = std.io.getStdIn();

    // Build the lists.
    processInput(stdin, &list_a, &list_b);
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
