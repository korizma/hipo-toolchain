# linker_error_extern_local_conflict

Purpose: verifies that relocatable linking rejects an object that imports a symbol which is only local in another object.

Expected result: linker exits with failure and prints `Extern local conflict: local_only`.
