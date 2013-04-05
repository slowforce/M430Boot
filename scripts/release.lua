-------------------------------------------------------------
-- Lua script generate firmware release file in json format
-- created by Tim Xu (slowforce@gmail.com) on Mar 19, 2013
-------------------------------------------------------------
require "lfs"

-------------------------------------------------
-- define output path and resource file path
-------------------------------------------------
outFilePath = "../Debug/"
verFilePath  = "../inc/version.h"

-------------------------------------------------
-- define ISR name
-- change to your ISR name accordingly
-------------------------------------------------
isrRoutineName = {}
isrRoutineName[1]  = nil
isrRoutineName[2]  = "buttonIsr"
isrRoutineName[3]  = "serial1PortTxIsr"
isrRoutineName[4]  = "serial1PortRxIsr"
isrRoutineName[5]  = "port1Isr"
isrRoutineName[6]  = "timerA1Isr"
isrRoutineName[7]  = "prvvMBTimerIRQHandler"
isrRoutineName[8]  = "ADC12ISR"
isrRoutineName[9]  = "prvvMBSerialTXIRQHandler"
isrRoutineName[10] = "prvvMBSerialRXIRQHandler"
isrRoutineName[11] = "wdtIsr"
isrRoutineName[12] = "comparatorAIsr"
isrRoutineName[13] = "currentloopTimerIsr"
isrRoutineName[14] = "timerB0Isr"
isrRoutineName[15] = "nmiIsr"

-------------------------------------------------
-- define ISR vector address
-------------------------------------------------
isrFlashAddr = {}
isrFlashAddr[1]  = "0xFFE0"
isrFlashAddr[2]  = "0xFFE2"
isrFlashAddr[3]  = "0xFFE4"
isrFlashAddr[4]  = "0xFFE6"
isrFlashAddr[5]  = "0xFFE8"
isrFlashAddr[6]  = "0xFFEA"
isrFlashAddr[7]  = "0xFFEC"
isrFlashAddr[8]  = "0xFFEE"
isrFlashAddr[9]  = "0xFFF0"
isrFlashAddr[10] = "0xFFF2"
isrFlashAddr[11] = "0xFFF4"
isrFlashAddr[12] = "0xFFF6"
isrFlashAddr[13] = "0xFFF8"
isrFlashAddr[14] = "0xFFFA"
isrFlashAddr[15] = "0xFFFC"

-------------------------------------------------
-- define ISR address
-------------------------------------------------
isrRoutineAddr = {}
isrRoutineAddr[1]  = "0xFFFF"
isrRoutineAddr[2]  = "0xFFFF"
isrRoutineAddr[3]  = "0xFFFF"
isrRoutineAddr[4]  = "0xFFFF"
isrRoutineAddr[5]  = "0xFFFF"
isrRoutineAddr[6]  = "0xFFFF"
isrRoutineAddr[7]  = "0xFFFF"
isrRoutineAddr[8]  = "0xFFFF"
isrRoutineAddr[9]  = "0xFFFF"
isrRoutineAddr[10] = "0xFFFF"
isrRoutineAddr[11] = "0xFFFF"
isrRoutineAddr[12] = "0xFFFF"
isrRoutineAddr[13] = "0xFFFF"
isrRoutineAddr[14] = "0xFFFF"
isrRoutineAddr[15] = "0xFFFF"

-------------------------------------------------
-- get map file last modification time
-------------------------------------------------
function getFileAccessTime(filePath)
	local attr = assert(lfs.attributes (filePath), "unable find file")
	for name, value in pairs(attr) do
		if (name == "modification") then
			return value
		end
	end
end

-------------------------------------------------
-- retrieve map file attributes
-------------------------------------------------
local f = assert(io.popen("dir \"..\\Debug\\\""), "unable find map file folder")
mapFileNamePrefix = assert(string.match(f:read("*a"), "%d+%s?([%w%s%-%_%.]+)%.map"), "unable find map file")
mapFileName = mapFileNamePrefix..".map"
mapFilePath = outFilePath..mapFileName
fdatetime = getFileAccessTime(mapFilePath)

-------------------------------------------------
-- build firmware release file name and path
-------------------------------------------------
relFileName  = mapFileNamePrefix.."_release"..".json"
relFilePath  = outFilePath..relFileName

-------------------------------------------------
-- create firmware release file
-------------------------------------------------
relFile = assert(io.open(relFilePath, "w+"), "unable open firmware release file for writing")
relFile:write("{\"ReleaseFile\": {\n")
relFile:write("  \"id\": \"Probe\",\n")
relFile:write("  \"date\":  \"")
relFile:write(fdatetime)
relFile:write("\",\n")

-------------------------------------------------
-- retrieve firmware version
-------------------------------------------------
vfile = assert(io.open(verFilePath, "r"), "unable open version file for reading")
linenum = 0
for line in vfile:lines() do
	linenum = linenum+1
	if (linenum == 4) then
		-- #define VERSION_NUMBER	40
		-- version = string.match(line, ".+%s+.+%s+.-(%d+).+")
		version = string.match(line, "VERSION_NUMBER%s+(%d+)")
		assert(version, "did not find firmware version, invalid version file")
	end
end

-------------------------------------------------
-- extract major and minor version
-------------------------------------------------
majorVer = math.floor((tonumber(version, 10))/0x100)
minorVer = (tonumber(version, 10))%0x100

relFile:write("  \"major\": ")
relFile:write(majorVer)
relFile:write(",\n")
relFile:write("  \"minor\": ")
relFile:write(minorVer)
relFile:write(",\n")

-------------------------------------------------
-- open map file for reading and parsing
--  1. get firmware reset vector address
--  2. get firmware size
--  3. get ISR address
-------------------------------------------------
linenum = 0
ifile = assert(io.open(mapFilePath,"r"), "unable open map file for reading")
for line in ifile:lines() do
	linenum = linenum+1
	if (linenum == 7) then	-- 1. find reset vector address
		-- find below line
		-- ENTRY POINT SYMBOL: "_c_int00"  address: 00004c82
		resetVectAddr = string.match(line, ".+%s.+%s.+%s.+%s+.+%s(%x+)")
		assert(resetVectAddr, "did not find reset vector address, invalid map file")
		-- relFile:write()
	elseif (linenum == 20) then	-- 2. find firmware size
		-- find below line
		--   FLASH                 00003200   0000cde0  0000209c  0000ad44  RWIX
		firmwareSize = string.match(line, "%s+FLASH%s+.+%s+.+%s+(.+)%s+.+%s+.+")
		assert(firmwareSize, "did not find firmware size, invalid map file")
		firmwareSize = tonumber(firmwareSize, 16)
		relFile:write("  \"length\": ")
		relFile:write(firmwareSize)
		relFile:write(",\n")
		relFile:write("  \"parameters\": {\n")
		relFile:write("    \"interruptVectorAddress\": [\n")
	else
		funcAddr, funcName = string.match(line, "(%w+)%s+(%w+)")
		if ( (funcAddr ~= nil) and (funcName ~= nil) ) then
			idx = 1
			while (idx < 16) do
				if (isrRoutineName[idx] ~= nil) then
					if (string.match(funcName, isrRoutineName[idx])) then
						isrRoutineAddr[idx] = funcAddr
						isrRoutineName[idx] = nil
					end
				end
				idx = idx+1
			end
		end
	end
end

-------------------------------------------------
-- write ISR address to firmware release file
-------------------------------------------------
for i=1,15,1 do
	relFile:write("      {\"vector\": \"")
	relFile:write(isrFlashAddr[i])
	relFile:write("\", \"entry\": \"")
	if (isrRoutineAddr[i] ~= "0xFFFF") then
		relFile:write("0x")
		relFile:write(string.sub(isrRoutineAddr[i], 5))
	else
		relFile:write(isrRoutineAddr[i])
	end
	relFile:write("\"},\n")
end

-------------------------------------------------
-- write reset vector address to release file
-------------------------------------------------
relFile:write("      {\"vector\": \"0xFFFE\", \"entry\": \"")
relFile:write("0x")
relFile:write(string.sub(resetVectAddr, 5))
relFile:write("\"}\n")
relFile:write("    ]\n")
relFile:write("  }\n")
relFile:write("}}\n")

relFile:close()
print("Firmware release file generated\n")
