# linker_relocatable_global_resolution

Purpose: verifies relocatable linking of one object that defines a global symbol and another that references it.

Expected result: linker succeeds, merges the `text` sections, keeps `target` and `start` global, leaves a relocation at offset `4`, and the linked object can be linked again.
