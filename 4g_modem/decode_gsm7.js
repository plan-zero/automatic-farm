var GSM7_Table = "000A,000C,000D,0020,0021,0022,0023,0024,0025,0026,0027,0028,0029,002A,002B,002C,002D,002E,002F,0030,0031,0032,0033,0034,0035,0036,0037,0038,0039,003A,003A,003B,003C,003D,003E,003F,0040,0041,0042,0043,0044,0045,0046,0047,0048,0049,004A,004B,004C,004D,004E,004F,0050,0051,0052,0053,0054,0055,0056,0057,0058,0059,005A,005B,005C,005D,005E,005F,0061,0062,0063,0064,0065,0066,0067,0068,0069,006A,006B,006C,006D,006E,006F,0070,0071,0072,0073,0074,0075,0076,0077,0078,0079,007A,007B,007C,007D,007E,00A0,00A1,00A3,00A4,00A5,00A7,00BF,00C4,00C5,00C6,00C7,00C9,00D1,00D6,00D8,00DC,00DF,00E0,00E4,00E5,00E6,00E8,00E9,00EC,00F1,00F2,00F6,00F8,00F9,00FC,0393,0394,0398,039B,039E,03A0,03A3,03A6,03A8,03A9,20AC".split(","),
    GSM7_Table_Extend =
    "007B,007D,005B,005D,007E,005C,005E,20AC,007C".split(",");
var specialChars = ["000D", "000A", "0009", "0000"],
    specialCharsIgnoreWrap = ["0009", "0000"];

function dec2hex(b) {
    return (b + 0).toString(16).toUpperCase()
}

function hex2char(b) {
    var a = "",
        b = parseInt(b, 16);
    65535 >= b ? a += String.fromCharCode(b) : 1114111 >= b && (b -= 65536, a += String.fromCharCode(55296 | b >> 10) + String.fromCharCode(56320 | b & 1023));
    return a
}


function decodeMessage(b) {
    if (!b) return "";
    var a = specialCharsIgnoreWrap;
    return b.replace(/([A-Fa-f0-9]{1,4})/g, function(b, e) {
        return -1 == a.indexOf(e) ? hex2char(e) : ""
    })
}

console.log(decodeMessage(process.argv[2]));