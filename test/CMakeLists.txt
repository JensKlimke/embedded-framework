# create executable
add_executable(StateMachineTest
            TimerTest.cpp
            StateMachineTest.cpp
        )

# include directories
target_include_directories(StateMachineTest PRIVATE
            ${PROJECT_SOURCE_DIR}/src
        )

# link libraries
target_link_libraries(StateMachineTest PRIVATE
            state
            framework
        )

# add gtest
add_gtest(StateMachineTest)
