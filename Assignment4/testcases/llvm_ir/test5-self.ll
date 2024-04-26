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
  %"value" = alloca i32
  store i32 10, i32* %"value"
  br label %"while.cond"
while.cond:
  %"1" = load i32, i32* %"value"
  %".4" = icmp sgt i32 %"1", 0
  br i1 %".4", label %"while.stmt", label %"while.end"
while.stmt:
  %"2" = load i32, i32* %"value"
  call void @"print_int"(i32 %"2")
  %".7" = alloca [2 x i8]
  store [2 x i8] c"\0a\00", [2 x i8]* %".7"
  %".9" = getelementptr [2 x i8], [2 x i8]* %".7", i32 0, i32 0
  call void @"print_string"(i8* %".9")
  %"3" = load i32, i32* %"value"
  %"4" = sub i32 %"3", 1
  store i32 %"4", i32* %"value"
  br label %"while.cond"
while.end:
  ret i32 0
}
