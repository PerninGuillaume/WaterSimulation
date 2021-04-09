  #include <cmath>
#include <vector>

  int main()
  {
    const int size = 256;
    std::vector<double> sinTable(size,0);
    
    #pragma omp parallel for
    for (auto& elt : sinTable) 
      elt = std::sin(2 * M_PI * n / size);
  
    // the table is now initialized
  }
