//#include "State.h"
//#include <iostream>
//
//inline void test()
//{
//	std::cout << "Initiating testing!\n";
//	try
//	{
//		//TilesContained_EmptyMap_AllZero
//		Map empty_map("empty");
//		for (int x = 0; x < empty_map.grid_width; x++)
//		{
//			for (int y = 0; y < empty_map.grid_height; y++)
//			{
//				if (empty_map.getTile(x, y) != 0)
//				{
//					throw std::runtime_error("Not all tiles were empty");
//				}
//			}
//		}
//		//Will need to rework this after developing more robust tilesystem
//		//TilesContained_LoadedFromFile_WithinRange:0,3
//		Map loaded_map("level1");
//		for (int x = 0; x < loaded_map.grid_width; x++)
//		{
//			for (int y = 0; y < loaded_map.grid_height; y++)
//			{
//				if (loaded_map.getTile(x, y) < 0 || loaded_map.getTile(x, y) > 3)
//				{
//					throw std::runtime_error("Not all tiles were within the range");
//				}
//			}
//		}
//		assert(1 == 1);
//		loaded_map.generatePathfinding(10, 10);
//		for (int x = 0; x < loaded_map.grid_width; x++)
//		{
//			for (int y = 0; y < loaded_map.grid_height; y++)
//			{
//				if (loaded_map.getPathTile(x, y) < 0 || loaded_map.getPathTile(x, y) > 3)
//				{
//					throw std::runtime_error("Pathfinding has values out of range?");
//				}
//			}
//		}
//	}
//	catch (std::exception const& e)
//	{
//		std::cerr << "Error! " << e.what() << "\n";
//	}
//}