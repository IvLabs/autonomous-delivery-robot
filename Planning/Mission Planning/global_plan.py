#!/usr/bin/env python
# coding: utf-8

# Import required libraries

# In[59]:


import osmnx as ox
import networkx as nx
import queue
import math
import priority_dict


# Import OSM map of VNIT and apply A* for mission planning
map_graph = ox.graph_from_file('/home/aniket/autonomous-delivery-robot/Planning/Vnit_map/CollegeMap.osm', simplify = True)
origin = ox.get_nearest_node(map_graph, (21.122, 79.045))
destination = list(map_graph.nodes())[15]
#fig, ax = ox.plot_graph(map_graph)


# This function follows the predecessor
# backpointers and generates the equivalent path from the
# origin as a list of vertex keys.
def get_path(origin_key, goal_key, predecessors):
    key = goal_key
    path = [goal_key]
    
    while (key != origin_key):
        key = predecessors[key]
        path.insert(0, key)
        
    return path


# Distance Heuristic Function
def distance_heuristic(state_key, goal_key, node_data):
    
    n1 = node_data[state_key]
    n2 = node_data[goal_key]
    
    #To find longitude and latitude of vertices
    
    long1 = n1['x']*math.pi/180
    lat1 = n1['y']*math.pi/180
    long2 = n2['x']*math.pi/180
    lat2 = n2['y']*math.pi/180
    
    # Use a spherical approximation of the earth for
    # estimating the distance between two points.
    r = 6371000
    x1 = r*math.cos(lat1)*math.cos(long1)
    y1 = r*math.cos(lat1)*math.sin(long1)
    z1 = r*math.sin(lat1)

    x2 = r*math.cos(lat2)*math.cos(long2)
    y2 = r*math.cos(lat2)*math.sin(long2)
    z2 = r*math.sin(lat2)

    d = ((x2-x1)**2 + (y2-y1)**2 + (z2-z1)**2)**0.5
    
    return d


# A* function
def A_star(origin_key, goal_key, graph):
    
    # min heap for open vertices
    open_queue = priority_dict.priority_dict({})
    #closed dictionary
    closed_dict = {}
    #predecessor dictionary
    predecessors = {}
    #best cost of each vertices
    costs = {}
    
    #get spatial data of nodes
    node_data = graph.nodes(True)
    
    costs[origin_key] = 0.0
    open_queue[origin_key] = distance_heuristic(origin_key, goal_key, node_data)
    
    #iterate through open_queue through all nodes till goal is found
    goal_found = False
    while (open_queue):
        # pop the initial vertex
        u, u_heu = open_queue.pop_smallest()
        u_cost = costs[u]
        # check condition if the start vertex is goal
        if u == goal_key:
            goal_found = True
            break
            
        for edge in graph.out_edges([u], data = True):
            # out_edges provide information about the edges. 
            #The return value of this is a list of tuples, each of which represent an outgoing edge. 
            #The second element of each tuple is the outgoing vertex at the other end of the edge.
            
            v = edge[1]          
            
            if v in closed_dict:
                continue
            
            uv_cost = edge[2]['length']
            
            if v not in open_queue:
                costs[v] = u_cost + uv_cost
                open_queue[v] = u_cost + uv_cost + distance_heuristic(v, goal_key, node_data)
                predecessors[v] = u
            else:
                v_cost = costs[v]
                if u_cost + uv_cost + distance_heuristic(v, goal_key, node_data) < v_cost:
                    open_queue[v] = u_cost + uv_cost + distance_heuristic(v, goal_key, node_data)
                    costs[v] = u_cost + uv_cost
                    predecessors[v] = u
                    
            closed_dict[v] = -1
            
    # If we get through entire priority queue without finding the goal,
    # something is wrong.
    if not goal_found:
        raise ValueError("Goal not found in search.")
    
    # Construct the path from the predecessors dictionary.
    return get_path(origin_key, goal_key, predecessors)               

path = A_star(origin, destination, map_graph)
print(path)        
#fig, ax = ox.plot_graph_route(map_graph, path) 


# In[ ]:




