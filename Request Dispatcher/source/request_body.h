struct RequestBody {
	char* dll_name; // Name of a dynamically loaded library
	char* func_name; // Name of a function to call from the DLL
	char** func_args; // Arguments to pass to the function
};
