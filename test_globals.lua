-- Test the exposed global variable
print("Global value from C++: " .. test.globalValue)

-- Modify the global value
test.globalValue = 100
print("Modified global value from C++: " .. test.globalValue)
