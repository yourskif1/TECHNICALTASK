// findfile.h

#ifndef FINDFILE_H
#define FINDFILE_H

#include <string>
#include <vector>

extern std::vector<std::string> pendingDirectories;

void findFile(const std::string& target);
void parallelSearch(const std::string& target, int numThreads);

#endif // FINDFILE_H
