/world/New()
    ..()

    world.log << "testing read\n"

    var/datum/lu_blob/test_file = lu_openfile("test.bin")
    var/c = test_file.read8()
    var/r = 0
    for(var/i = 1, i <= c, i++)
        var/f = test_file.read16()
        r = test_method(r)
    var/slen = test_file.read8()
    var/date = test_file.read(slen)
    world.log << "date: [date]"
    var/clk = test_file.readfloat()
    var/time = test_file.readfloat(TRUE)
    world.log << "clk: [clk], time: [time]"
    test_file.close()

    world.log << "\ntesting write\n"
    // test writing
    var/datum/lu_blob/test_write = lu_openfile("test2.bin", OPEN_WRITE)
    var/str = "testing 1234"
    var/ts = world.realtime
    test_write.write8(length(str))
    test_write.write(str)
    test_write.write16(world.byond_version)
    test_write.write16(world.byond_build)
    test_write.writefloat(ts)
    test_write.close()

    world.log << "\ntesting readback\n"
    test_file = lu_openfile("test2.bin", OPEN_READ)
    var/strv = test_file.readstr8()
    world.log << "[(str == strv) ? "OK" : "FAIL"] [str] [strv]"
    var/maj = test_file.read16()
    var/min = test_file.read16()
    var/vok = (maj == world.byond_version) && (min == world.byond_build)
    world.log << "[vok ? "OK" : "FAIL"] [world.byond_version].[world.byond_build] [maj].[min]"
    var/tsv = test_file.readfloat()
    world.log << "[(tsv == ts) ? "OK" : "FAIL"] [time2text(ts)] [time2text(tsv)]"

    shutdown()


/world/proc/test_method(x)
    x ^= 0xAA55
    x *= 3
    x += 42
    x &= 0xFFFF
    x = (x << 3) | (x >> 12)
    x &= 0xFFFF
    return x