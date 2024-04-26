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
  %"y" = alloca i32
  store i32 5, i32* %"y"
  %"1" = load i32, i32* %"y"
  call void @"print_int"(i32 %"1")
  %".4" = alloca [2 x i8]
  store [2 x i8] c"\0a\00", [2 x i8]* %".4"
  %".6" = getelementptr [2 x i8], [2 x i8]* %".4", i32 0, i32 0
  call void @"print_string"(i8* %".6")
  br label %"if.cond"
if.cond:
  %"2" = load i32, i32* %"y"
  %".9" = icmp sgt i32 %"2", 0
  br i1 %".9", label %"if.then", label %"if.else"
if.then:
  %"is_y_positive" = alloca i32
  store i32 1, i32* %"is_y_positive"
  %"3" = load i32, i32* %"is_y_positive"
  call void @"print_bool"(i32 %"3")
  br label %"if.end"
if.else:
  %"is_y_positive.1" = alloca i32
  store i32 0, i32* %"is_y_positive.1"
  %"4" = load i32, i32* %"is_y_positive.1"
  call void @"print_bool"(i32 %"4")
  br label %"if.end"
if.end:
  ret i32 0
}
