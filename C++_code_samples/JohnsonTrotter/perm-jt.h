/******************************************************************************/
/*!
\file   perm-jt.h
\author Richard Shen
\par    email: r.shen\@digipen.edu
\par    DigiPen login: r.shen
\par    Course: CS330
\par    Lab 2
\date   06/06/2017
\brief  
    This file contains the interface of PermJohnsonTrotter
    for the assignment.
    
  Hours spent on this assignment: 

  Specific portions that gave you the most trouble:
    Using index incorrectly took sometime to debug
*/
/******************************************************************************/
#ifndef PERM_JOHNSON_TROTTER_H
#define PERM_JOHNSON_TROTTER_H
#include <vector>

class PermJohnsonTrotter {
	private:

  bool IsMobile();
  int findLargestMobileIndex();
  void reverseDir(int largestIndex);
  void updateMobility();
  
  const int size_;
  std::vector<int> elements_;
  std::vector<int> dir_;
  std::vector<bool> mobile_;
	
	public:
		PermJohnsonTrotter(int size);
		bool Next();
		std::vector<int> const& Get() const;
};
#endif
