function toDbgString(val)
	if type(val) == 'table' then
		return toJSON(val)
	else
		return tostring(val)
	end
end

function assertEq(val1, val2)
	if val1 ~= val2 then
		outputDebugString('Assertion failed: '..toDbgString(val1)..' <> '..toDbgString(val2)..'\n'..debug.traceback(), 1)
	end
end

function assertTrue(val)
	if not val then
		outputDebugString('Assertion failed: '..toDbgString(val)..'\n'..debug.traceback(), 1)
	end
end

-- compressJSON test
local tbl = {
	{ foo = 'bar' },
	{ foo = 'bar2' },
}
assertEq(compressJSON(toJSON(tbl)), '[[{"foo":"bar"},{"foo":"bar2"}]]')

-- zlibCompress/zlibUncompress test
assertEq(zlibUncompress(zlibCompress('foobar')), 'foobar')

-- fileFind tests
assertEq(toJSON(fileFind()), '[ [ "meta.xml", "server.lua" ] ]')
assertEq(toJSON(fileFind('*.xml')), '[ [ "meta.xml" ] ]')

-- fileModTime test
assertEq(type(fileModTime('meta.xml')), 'number')

-- curl tests
local curl = curlInit('http://example.com')
assertEq(type(curl), 'userdata')
assertEq(curlEscape(curl, 'abc '), 'abc%20')
assertEq(curlSetopt(curl, 'CURLOPT_TIMEOUT', 30), true)
assertEq(curlPerform(curl), true)
assertEq(curlClose(curl), true)
