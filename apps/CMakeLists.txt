add_executable(app app.cpp)
target_link_libraries(app PRIVATE decompiler_lib)
set_project_warnings(app)
