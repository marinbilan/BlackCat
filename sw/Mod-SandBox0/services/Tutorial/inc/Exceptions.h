


/*
Learn Modern C++ Exceptions - 140. Try and Catch Blocks
*/
void testExcMethod() 
{
    std::vector<int> vec;

    /*
    try {
       std::cout << vec.at(2) << '\n';  // Throw exception
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << '\n';
    }
    */

   try {
      std::cout << vec.at(2) << '\n';
   } catch (const std::out_of_range& e) {
      std::cout << "Exception caught: " << e.what() << '\n';
   } catch (const std::exception& e) {
      std::cout << "Generic Exception caught: " << e.what() << '\n';
   }
}