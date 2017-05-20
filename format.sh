#!/bin/bash

STYLE="{
	\"BasedOnStyle\": \"llvm\",
	\"IndentWidth\": 8,
	\"TabWidth\": 8,
	\"ContinuationIndentWidth\": 8,
	\"UseTab\": \"ForIndentation\",
	\"BreakBeforeBraces\": \"Attach\",
	\"AlwaysBreakTemplateDeclarations\": \"true\",
	\"ColumnLimit\": 100,
	\"PointerBindsToType\": \"true\",
	\"AccessModifierOffset\": -8,
	\"ConstructorInitializerIndentWidth\": 8,
}"

function format_file() {
	if !diff -u <(cat "$1") <(clang-format -style="$STYLE" $1) &> "/dev/null"; then
		echo formatting "$1"
		clang-format -style="$STYLE" -i "$1"
	fi
}

for FILE in $(find include -iname "*.[hc]pp"); do format_file "$FILE"; done
for FILE in $(find lib     -iname "*.[hc]pp"); do format_file "$FILE"; done
for FILE in $(find app     -iname "*.[hc]pp"); do format_file "$FILE"; done
for FILE in $(find test    -iname "*.[hc]pp"); do format_file "$FILE"; done


