  
#ifndef VERTEX_CACHE_H

#define VERTEX_CACHE_H

#include <iostream>

class VertexCache
{
	
public:
	
	VertexCache(int size);
	VertexCache();

	~VertexCache();
	
	bool InCache(int entry);
	int AddEntry(int entry);
	void Copy(VertexCache* inVcache);
	void Set(int index, int value);
	int At(int index);
	void Clear();

private:

  int *entries;
  int numEntries;

};

#endif
