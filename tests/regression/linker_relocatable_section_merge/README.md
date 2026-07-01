# linker_relocatable_section_merge

Purpose: verifies that relocatable linking concatenates sections with the same name in input order.

Expected result: linker succeeds and the merged `text` section contains both words, left object first.
