


void testExcMethod() 
{
    std::vector<int> vec;

    try {
       std::cout << vec[2] << '\n';  // Throw exception
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << '\n';
    }
}