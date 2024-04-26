; ModuleID = ""
target triple = "unknown-unknown-unknown"
target datalayout = ""

declare i32* @"array_of_string"(i8* %".1") 

declare i8* @"string_of_array"(i32* %".1") 

declare i32 @"length_of_string"(i8* %".1") 

declare i8* @"string_of_int"(i32 %".1") 

declare i8* @"string_cat"(i8* %".1", i8* %".2") 

declare void @"print_string"(i8* %".1") 

declare void @"print_int"(i32 %".1") 

declare void @"print_bool"(i32 %".1") 

define i32 @"main"() 
{
entry:
  br label %"for.init"
for.init:
  %"i" = alloca i32
  store i32 0, i32* %"i"
  br label %"for.cond"
for.cond:
  %"1" = load i32, i32* %"i"
  %".5" = icmp slt i32 %"1", 10
  br i1 %".5", label %"for.stmt", label %"for.end"
for.stmt:
  %"2" = load i32, i32* %"i"
  call void @"print_int"(i32 %"2")
  %".8" = alloca [2 x i8]
  store [2 x i8] c"\0a\00", [2 x i8]* %".8"
  %".10" = getelementptr [2 x i8], [2 x i8]* %".8", i32 0, i32 0
  call void @"print_string"(i8* %".10")
  br label %"for.update"
for.update:
  %"3" = load i32, i32* %"i"
  %"4" = add i32 %"3", 1
  store i32 %"4", i32* %"i"
  br label %"for.cond"
for.end:
  ret i32 0
}
