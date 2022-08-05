#include "function.h"
using namespace std;

float distanceCheck(vector<int> route, vector<vector<float>> &distMatrix)
{
    float totaldistance = 0;
    for (int j = 0, k = 1; j < route.size() - 1; j++, k++)
    {
        totaldistance += distMatrix[route[j]][route[k]];
    }
    return totaldistance;
}

int checkCapacity(vector<int> route, vector<vector<int>> &customerdata)
{
    int capacity = 0;
    for (int i = 0; i < route.size(); i++)
    {
        capacity += customerdata[route[i]][3];
    }
    return capacity;
}
float objfunc(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int vehicle, vector<float> distanceofEachfleet, int externalTransportationCost)
{
    float totalvariablecost = 0;
    for (int x = 0; x < vehicle; x++)
    {
        totalvariablecost += distanceofEachfleet[x] * truckarr[x].variable_cost;
    }
    int totalfixedcostofTruck = 0;
    for (int x = 0; x < vehicle; x++)
    {
        totalfixedcostofTruck += truckarr[x].fixed_cost;
    }
    float costofInternalfleet = totalfixedcostofTruck + totalvariablecost;
    float objvalue = 0;
    objvalue = costofInternalfleet + externalTransportationCost;
    return objvalue;
}

int findRouteNo(int node, vector<vector<int>> route)
{
    vector<int>::iterator it;
    for (int i = 0; i < route.size(); i++)
    {
        it = find(route[i].begin(), route[i].end(), node);
        if (it != route[i].end())
        {
            return i;
        }
    }
    return 9999999;
}

int findMinFromMatrix(vector<vector<float>> matrix)
{
    float minm = matrix[0][0];
    int minIndex = 0;

    // Run the inner loop for columns
    for (int j = 0; j < matrix.size(); j++)
    {

        // check if any element is smaller
        // than the minimum element of the column
        // and replace it
        if (matrix[j][0] < minm)
        {
            minm = matrix[j][0];
            minIndex = j;
        }
    }
    return minIndex;
}

int findMaxFromMatrix(vector<vector<float>> matrix)
{
    float max = matrix[0][0];
    int maxIndex = 0;

    // Run the inner loop for columns
    for (int j = 0; j < matrix.size(); j++)
    {

        // check if any element is smaller
        // than the minimum element of the column
        // and replace it
        if (matrix[j][0] > max)
        {
            max = matrix[j][0];
            maxIndex = j;
        }
    }
    return maxIndex;
}

sol VRPPC::adjacentSwapping(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int externalTransportationCost, sol s1, float randomMultipier)
{
    sol s;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    vector<float> prevRouteDistances;
    vector<float> newRouteDistances;
    prevRouteDistances = s1.routeDistance;

    int var = 0;
    float distance = 0, distance_1 = 0;
    for (int x = 0; x < vehicle; x++)
    {
        routeCopy = route[x];
        for (int i = 1; i < route[x].size() - 2; i++)
        {
            var = routeCopy[i];
            routeCopy[i] = routeCopy[i + 1];
            routeCopy[i + 1] = var;
            distance = distanceCheck(routeCopy, distMatrix);
            if (distance < prevRouteDistances[x] * randomMultipier)
            {
                route[x] = routeCopy;
                prevRouteDistances[x] = distance;
            }
            else
            {
                routeCopy = route[x];
            }
        }
        newRouteDistances.push_back(prevRouteDistances[x]);
    }
    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, newRouteDistances, externalTransportationCost);
    s.vehicleroute = route;
    s.initcost = objvalue;
    s.routeDistance = newRouteDistances;
    s.ext_transportcost = s1.ext_transportcost;
    s.notvisited = s1.notvisited;
    s.remcapacityofvehicle = s1.remcapacityofvehicle;
    s.visited = s1.visited;
    return s;
}

sol VRPPC::one_oneSwapping(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int externalTransportationCost, sol s1, float randomMultipier)
{
    sol s;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    int var;
    float distance = 0, distance_1 = 0;
    vector<float> prevRouteDistances;
    prevRouteDistances = s1.routeDistance;
    vector<float> newRouteDistances;
    vector<float> storeDist;
    for (int x = 0; x < vehicle; x++)
    {
        routeCopy = route[x];
        if (route[x].size() == 3)
        {
            newRouteDistances.push_back(prevRouteDistances[x]);
            continue;
        }
        for (int i = 1; i < route[x].size() - 1; i++)
        {
            for (int j = 1; j < route[x].size() - 1; j++)
            {
                if (i == j)
                {
                    continue;
                }
                var = routeCopy[i];
                routeCopy[i] = routeCopy[j];
                routeCopy[j] = var;
                distance = distanceCheck(routeCopy, distMatrix);
                storeDist.push_back(distance);
                routeCopy = route[x];
            }

            float min = *min_element(storeDist.begin(), storeDist.end());
            int min_index = min_element(storeDist.begin(), storeDist.end()) - storeDist.begin();
            if (min < prevRouteDistances[x] * randomMultipier)
            {
                var = route[x][i];
                route[x][i] = route[x][min_index + 2];
                route[x][min_index + 2] = var;
                prevRouteDistances[x] = min;
            }
            storeDist.clear();
        }
        newRouteDistances.push_back(prevRouteDistances[x]);
    }
    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, newRouteDistances, externalTransportationCost);
    s.vehicleroute = route;
    s.initcost = objvalue;
    s.routeDistance = newRouteDistances;
    s.ext_transportcost = s1.ext_transportcost;
    s.notvisited = s1.notvisited;
    s.remcapacityofvehicle = s1.remcapacityofvehicle;
    s.visited = s1.visited;
    return s;
}

sol VRPPC::singleInsertionSwapping(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int externalTransportationCost, sol s1, float randomMultipier)
{
    sol s;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    int var;
    float distance = 0, distance_1 = 0, objValueCopy = s1.initcost;
    vector<float> prevRouteDistances;
    prevRouteDistances = s1.routeDistance;
    vector<float> newRouteDistances;
    vector<float> storeDist;

    vector<vector<int>> routesAfterInsert;
    for (int x = 0; x < vehicle; x++)
    {
        if (route[x].size() == 3)
        {
            newRouteDistances.push_back(prevRouteDistances[x]);
            continue;
        }
        for (int i = 1; i < route[x].size() - 1; i++)
        {
            for (int j = 0; j < route[x].size() - 1; j++)
            {
                routeCopy = route[x];
                if (j <= i - 2)
                {
                    routeCopy.insert(routeCopy.begin() + j + 1, routeCopy[i]);
                    routeCopy.erase(routeCopy.begin() + (i + 1));
                }
                if (j == i - 1)
                {
                    continue;
                }
                if (j == i)
                {
                    continue;
                }
                if (j >= i + 1)
                {
                    routeCopy.insert(routeCopy.begin() + j + 1, routeCopy[i]);
                    routeCopy.erase(routeCopy.begin() + (i));
                }
                routesAfterInsert.push_back(routeCopy);
                distance = distanceCheck(routeCopy, distMatrix);
                storeDist.push_back(distance);
            }
            float min = *min_element(storeDist.begin(), storeDist.end());
            int min_index = min_element(storeDist.begin(), storeDist.end()) - storeDist.begin();
            if (min < prevRouteDistances[x])
            {
                prevRouteDistances[x] = min;
                route[x] = routesAfterInsert[min_index];
            }
            storeDist.clear();
            routesAfterInsert.clear();
        }
        newRouteDistances.push_back(prevRouteDistances[x]);
        float objectiveVal = objfunc(customerdata, distMatrix, truckarr, vehicle, prevRouteDistances, s1.ext_transportcost);
        if (objectiveVal < objValueCopy * randomMultipier)
        {
            s1.vehicleroute = route;
        }
        else
        {
            route = s1.vehicleroute;
        }
    }
    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, newRouteDistances, externalTransportationCost);

    s.vehicleroute = route;
    s.initcost = objvalue;
    s.routeDistance = newRouteDistances;
    s.ext_transportcost = s1.ext_transportcost;
    s.notvisited = s1.notvisited;
    s.remcapacityofvehicle = s1.remcapacityofvehicle;
    s.visited = s1.visited;
    return s;
}

sol VRPPC::externalNodeInsertion(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int externalTransportationCost, sol s1, float randomMultipier)
{
    sol s;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    int var;
    float distance = 0, distance_1 = 0, objValueCopy = s1.initcost;
    vector<float> prevRouteDistances, prevRouteDistances1;
    prevRouteDistances = s1.routeDistance;
    prevRouteDistances1 = s1.routeDistance;
    vector<int> remcap;
    remcap = s1.remcapacityofvehicle;
    vector<float> newRouteDistances;
    vector<float> storeDist;
    vector<int> recordOfInsertedNodes;
    vector<int> trackOfConsideration(2, 0);
    vector<vector<float>> newRouteDistances2d(vehicle, vector<float>(s1.notvisited.size(), 0));
    int index, flag = 0;
    int totalCapacity;
    int pseudocapacity;
    float pseudodistance;
    vector<int> track1;   // track of nodes
    vector<float> track2; // track of min distances
    vector<int> track3;   // track of route

    for (int k = 0; k < s1.notvisited.size(); k++)
    {
        for (int i = 0; i < route.size(); i++)
        {
            routeCopy = route[i];
            pseudodistance = s1.routeDistance[i];        // for checking and updating within loops
            pseudocapacity = s1.remcapacityofvehicle[i]; // for checking and updating within loops
            for (int j = 0; j < route[i].size() - 1; j++)
            {
                routeCopy.insert(routeCopy.begin() + j + 1, s1.notvisited[k]);
                totalCapacity = checkCapacity(routeCopy, customerdata);
                distance = distanceCheck(routeCopy, distMatrix);
                if (totalCapacity <= truckarr[i].capacity)
                {
                    if (distance <= pseudodistance)
                    {
                        flag = 1;
                        trackOfConsideration[0] = i;     // route number
                        trackOfConsideration[1] = j + 1; // position
                        pseudodistance = distance;
                    }
                }
                routeCopy.erase(routeCopy.begin() + j + 1);
            }
        }
        if (flag == 1)
        {
            route[trackOfConsideration[0]].insert(route[trackOfConsideration[0]].begin() + trackOfConsideration[1], s1.notvisited[k]);
            s1.routeDistance[trackOfConsideration[0]] = distanceCheck(route[trackOfConsideration[0]], distMatrix);
            s1.remcapacityofvehicle[trackOfConsideration[0]] = pseudocapacity;
            recordOfInsertedNodes.push_back(s1.notvisited[k]);
        }
        float objectiveVal = objfunc(customerdata, distMatrix, truckarr, vehicle, s1.routeDistance, s1.ext_transportcost);
        if (objectiveVal < objValueCopy * randomMultipier)
        {
            s1.vehicleroute = route;
        }
        else
        {
            route = s1.vehicleroute;
        }
        flag = 0;
    }

    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, s1.routeDistance, externalTransportationCost);

    s.vehicleroute = route;
    s.initcost = objvalue;
    s.routeDistance = s1.routeDistance;
    s.ext_transportcost = externalTransportationCost;
    s.notvisited = s1.notvisited;
    s.remcapacityofvehicle = s1.remcapacityofvehicle;
    s.visited = s1.visited;
    s.insertedNode = recordOfInsertedNodes;
    return s;
}

sol VRPPC::externalNodeSwapping(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, int externalTransportationCost, sol s1, float randomMultipier)
{
    sol s;
    int var;
    int count;
    float distance = 0, distance_1 = 0;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    vector<float> prevRouteDistances;
    prevRouteDistances = s1.routeDistance;
    vector<float> newRouteDistances;
    vector<float> storeDist;
    vector<int> recordOfInsertedNodes;
    externalTransportationCost = s1.ext_transportcost;
    recordOfInsertedNodes = s1.insertedNode;
    for (int i = 0; i < recordOfInsertedNodes.size(); i++)
    {
        vector<int>::iterator position = find(s1.notvisited.begin(), s1.notvisited.end(), recordOfInsertedNodes[i]);
        if (position != s1.notvisited.end())
        {
            s1.notvisited.erase(position);
        }
    }
    vector<vector<float>> newRouteDistances2d(vehicle, vector<float>(s1.notvisited.size(), 0));
    vector<int> trackof_j;
    vector<int> trackof_i;
    vector<vector<float>> trackOfrecords(vehicle, vector<float>(3));

    for (int k = 0; k < s1.notvisited.size(); k++)
    {
        for (int i = 0; i < route.size(); i++)
        {
            routeCopy = route[i];
            for (int j = 1; j < route[i].size() - 1; j++)
            {
                storeDist.push_back(distMatrix[routeCopy[j]][s1.notvisited[k]]);
                trackof_j.push_back(j);
            }
            float min = *min_element(storeDist.begin(), storeDist.end());
            int min_index = min_element(storeDist.begin(), storeDist.end()) - storeDist.begin();
            trackOfrecords[i][0] = (min);
            trackOfrecords[i][1] = (trackof_j[min_index]);
            trackOfrecords[i][2] = (i);
            storeDist.clear();
            trackof_j.clear();
        }
        int final_minIndex = findMinFromMatrix(trackOfrecords);
        routeCopy1 = route[int(trackOfrecords[final_minIndex][2])];
        routeCopy = route[int(trackOfrecords[final_minIndex][2])];

        routeCopy1[int(trackOfrecords[final_minIndex][1])] = s1.notvisited[k];
        float dist = distanceCheck(routeCopy1, distMatrix);
        int capacity = checkCapacity(routeCopy1, customerdata);
        if (capacity <= truckarr[int(trackOfrecords[final_minIndex][2])].capacity)
        {
            if (dist < prevRouteDistances[int(trackOfrecords[final_minIndex][2])] * randomMultipier)
            {
                route[int(trackOfrecords[final_minIndex][2])] = routeCopy1;
                newRouteDistances2d[int(trackOfrecords[final_minIndex][2])][k] = dist;
                s1.notvisited[k] = routeCopy[int(trackOfrecords[final_minIndex][1])];
                externalTransportationCost = externalTransportationCost + customerdata[routeCopy1[int(trackOfrecords[final_minIndex][1])]][4] - customerdata[s1.notvisited[k]][4];
                prevRouteDistances[int(trackOfrecords[final_minIndex][2])] = dist;
                s1.remcapacityofvehicle[int(trackOfrecords[final_minIndex][2])] = truckarr[int(trackOfrecords[final_minIndex][2])].capacity - capacity;
            }
        }
        for (int x = 0; x < trackOfrecords.size(); x++)
        {
            for (int y = 0; y < trackOfrecords[x].size(); y++)
            {
                fill(trackOfrecords[x].begin(), trackOfrecords[x].end(), 0);
            }
        }
    }
    for (int i = 0; i < newRouteDistances2d.size(); i++)
    {
        count = 0;
        for (int j = 0; j < newRouteDistances2d[i].size(); j++)
        {
            if (newRouteDistances2d[i][j] == 0)
            {
                count++;
            }
            else
            {
                newRouteDistances.push_back(newRouteDistances2d[i][j]);
            }
        }
        if (count == newRouteDistances2d[i].size())
        {
            newRouteDistances.push_back(prevRouteDistances[i]);
        }
    }
    vector<int> temp;
    unordered_set<int> set1(s1.notvisited.begin(), s1.notvisited.end());
    for (int x = 1; x <= customer; x++)
    {
        if (set1.find(x) == set1.end())
        {
            temp.push_back(x);
        }
    }
    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, newRouteDistances, externalTransportationCost);
    s.vehicleroute = route;
    s.initcost = objvalue;
    s.routeDistance = newRouteDistances;
    s.ext_transportcost = externalTransportationCost;
    s.notvisited = s1.notvisited;
    s.remcapacityofvehicle = s1.remcapacityofvehicle;
    s.visited = temp;
    s.insertedNode = recordOfInsertedNodes;

    return s;
}

sol VRPPC::swapWithMaxLink(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, sol s1, float randomMultipier)
{
    sol s2;
    float pseudoDist1, pseudoDist2;
    int pseudoRemCapacity;
    int flag = 0;
    vector<float> deviation;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<int> trackOfConsideration(3, 0);
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    for (int i = 0; i < route.size(); i++)
    {
        for (int j = 0; j < route[i].size() - 2; j++)
        {
            deviation.push_back(distMatrix[route[i][j]][route[i][j + 1]] + distMatrix[route[i][j + 1]][route[i][j + 2]]);
        }
        float maxDeviation = *max_element(deviation.begin(), deviation.end());
        int index_maxDeviation = max_element(deviation.begin(), deviation.end()) - deviation.begin();
        int appNode = route[i][index_maxDeviation + 1]; // appropriate node for swapping
        for (int k = 0; k < route.size(); k++)
        {
            if (k == i)
                continue;
            pseudoDist1 = s1.routeDistance[k];              // for checking and updating within loops
            pseudoDist2 = s1.routeDistance[i];              // for checking and updating within loops
            pseudoRemCapacity = s1.remcapacityofvehicle[k]; // for checking and updating within loops
            for (int x = 0; x < route[k].size() - 2; x++)
            {
                routeCopy1 = route[i];
                routeCopy = route[k];
                float oldDistance = distanceCheck(routeCopy, distMatrix) + distanceCheck(routeCopy1, distMatrix); // keeping sum of distance of two routes
                int tempNode = routeCopy[x + 1];
                routeCopy[x + 1] = routeCopy1[index_maxDeviation + 1];
                routeCopy1[index_maxDeviation + 1] = tempNode;
                int capacity1 = checkCapacity(routeCopy, customerdata);
                int capacity2 = checkCapacity(routeCopy1, customerdata);
                float newDistance = distanceCheck(routeCopy, distMatrix) + distanceCheck(routeCopy1, distMatrix);
                if (truckarr[k].capacity >= capacity1 && truckarr[i].capacity >= capacity2)
                {
                    if (newDistance < oldDistance * randomMultipier)
                    {
                        route[i] = routeCopy1;
                        route[k] = routeCopy;
                    }
                }
            }
        }
        deviation.clear();
        flag = 0;
    }
    for (int i = 0; i < route.size(); i++)
    {
        s1.routeDistance[i] = distanceCheck(route[i], distMatrix);
    }

    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, s1.routeDistance, s1.ext_transportcost);

    s2.ext_transportcost = s1.ext_transportcost;
    s2.initcost = objvalue;
    s2.notvisited = s1.notvisited;
    s2.remcapacityofvehicle = s1.remcapacityofvehicle;
    s2.routeDistance = s1.routeDistance;
    s2.vehicleroute = route;
    s2.visited = s1.visited;
    return s2;
}

sol VRPPC::insertWithMaxLink(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, sol s1, float randomMultipier)
{
    sol s2;
    float pseudoDist, minDiff;
    int pseudoRemCapacity;
    int flag = 0;
    vector<float> deviation;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<float> routeDistance(vehicle);
    routeDistance = s1.routeDistance;
    vector<int> trackOfConsideration(3, 0);
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    float objValueCopy = s1.initcost;
    for (int i = 0; i < route.size(); i++)
    {
        for (int j = 0; j < route[i].size() - 2; j++)
        {
            deviation.push_back(distMatrix[route[i][j]][route[i][j + 1]] + distMatrix[route[i][j + 1]][route[i][j + 2]]);
        }

        float maxDeviation = *max_element(deviation.begin(), deviation.end());
        int index_maxDeviation = max_element(deviation.begin(), deviation.end()) - deviation.begin();
        int appNode = route[i][index_maxDeviation + 1]; // appropriate node for inserting
        cout << "\nIteration : " << i << " and " << appNode << " -- Appropriate Node" << endl;
        for (int i = 0; i < route.size(); i++)
        {
            for (int j = 0; j < route[i].size(); j++)
            {
                cout << route[i][j] << " ";
            }
            cout << endl;
        }
        for (int k = 0; k < route.size(); k++)
        {
            if (k == i)
                continue;
            pseudoDist = s1.routeDistance[k];               // for checking and updating within loops
            pseudoRemCapacity = s1.remcapacityofvehicle[k]; // for checking and updating within loops
            routeCopy = route[k];
            routeCopy1 = route[i];
            float oldDistance = distanceCheck(routeCopy, distMatrix) + distanceCheck(routeCopy1, distMatrix); // keeping sum of distance of two routes
            for (int x = 0; x < routeCopy.size() - 1; x++)
            {
                routeCopy.insert(routeCopy.begin() + x + 1, appNode);
                routeCopy1.erase(remove(routeCopy1.begin(), routeCopy1.end(), appNode), routeCopy1.end());
                float newDistance = distanceCheck(routeCopy, distMatrix) + distanceCheck(routeCopy1, distMatrix);
                int capacity = checkCapacity(routeCopy, customerdata);
                // keeping a copy of original route
                if (truckarr[k].capacity >= capacity)
                {
                    if (newDistance <= oldDistance)
                    {
                        route[i] = routeCopy1;
                        route[k] = routeCopy;
                        for (int m = 0; m < route.size(); m++)
                        {
                            if (m == i || m == k)
                                continue;
                            route[m] = s1.vehicleroute[m];
                        }
                    }
                }
                routeCopy1.insert(routeCopy1.begin() + index_maxDeviation + 1, appNode);
                routeCopy.erase(remove(routeCopy.begin(), routeCopy.end(), appNode), routeCopy.end());
            }
        }
        for (int i = 0; i < route.size(); i++)
        {
            routeDistance[i] = distanceCheck(route[i], distMatrix);
        }
        float objectiveVal = objfunc(customerdata, distMatrix, truckarr, vehicle, routeDistance, s1.ext_transportcost);
        if (objectiveVal < objValueCopy * randomMultipier)
        {
            s1.vehicleroute = route;
        }
        else
        {
            route = s1.vehicleroute;
        }
        deviation.clear();
        flag = 0;
    }
    for (int i = 0; i < route.size(); i++)
    {
        s1.routeDistance[i] = distanceCheck(route[i], distMatrix);
    }

    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, s1.routeDistance, s1.ext_transportcost);

    s2.ext_transportcost = s1.ext_transportcost;
    s2.initcost = objvalue;
    s2.notvisited = s1.notvisited;
    s2.remcapacityofvehicle = s1.remcapacityofvehicle;
    s2.routeDistance = s1.routeDistance;
    s2.vehicleroute = route;
    s2.visited = s1.visited;
    return s2;
}
void VRPPC::objectiveFunction(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr)
{
    vector<vector<int>> route(vehicle);
    route[0] = {0, 38, 9, 30, 34, 50, 16, 21, 29, 2, 11, 0};
    route[1] = {0, 12, 37, 44, 15, 45, 33, 39, 10, 49, 5, 46, 0};
    route[2] = {0, 32, 1, 22, 20, 35, 36, 3, 28, 31, 8, 26, 7, 48, 0};
    route[3] = {0, 4, 17, 42, 19, 40, 41, 13, 25, 14, 24, 43, 23, 6, 0};
    vector<int> externalNode = {18, 27, 47};
    int extCost = 0;
    for (int i = 0; i < externalNode.size(); i++)
    {
        extCost += customerdata[externalNode[i]][4];
    }
    vector<float> routeDistance(vehicle);
    for (int i = 0; i < route.size(); i++)
    {
        routeDistance[i] = distanceCheck(route[i], distMatrix);
    }
    cout << "\nObjective value of optimal Node = " << objfunc(customerdata, distMatrix, truckarr, vehicle, routeDistance, extCost);
}
sol VRPPC::nbdFunction(vector<vector<int>> &customerdata, vector<vector<float>> &distMatrix, truck *truckarr, sol s1, float randomMultiplier)
{
    sol s2;
    float pseudoDist, minDiff;
    int pseudoRemCapacity;
    int flag = 0;
    vector<float> deviation;
    vector<int> routeCopy;
    vector<int> routeCopy1;
    vector<vector<int>> route(vehicle);
    route = s1.vehicleroute;
    vector<vector<float>> distMatrixCopy = distMatrix;
    // nbd function will run for every nodes present in all routes
    for (int i = 0; i < vehicle; i++)
    {
        for (int j = 1; j < route[i].size() - 1; j++)
        {
            vector<pair<int, float>> eachRouteOfDistMatrix;
            for (int x = 0; x < distMatrixCopy[route[i][j]].size(); ++x)
            {
                float dist = distMatrixCopy[route[i][j]][x];
                eachRouteOfDistMatrix.push_back(make_pair(x, dist)); // make pair of index and route
            }
            sort(eachRouteOfDistMatrix.begin(), eachRouteOfDistMatrix.end());
            vector<int> neighboringNodes(3);
            for (int x = 0; x < 3; x++) // for checking if 2nd nearest or 3rd nearest are feasible  or  not
            {
                neighboringNodes[x] = eachRouteOfDistMatrix[x + 1].first;
            }
            for (int x = 0; x < neighboringNodes.size(); x++)
            {
                int rtNo = findRouteNo(neighboringNodes[x], route);
                if (rtNo == i)
                    continue;
                int appNode = neighboringNodes[x]; // it lies in route[rtNo]
                routeCopy = route[i];
                routeCopy1 = route[rtNo];
                cout << "\nAppNode -> " << appNode << " Route No -> " << rtNo;
                cout << "\nRouteCopy before insert\n";
                for (int m : routeCopy)
                {
                    cout << m << "  ";
                }
                float oldDistance = distanceCheck(routeCopy, distMatrix) + distanceCheck(routeCopy1, distMatrix); // keeping sum of distance of two routes
                cout << "\nOld Distance before insertion : " << oldDistance;
                for (int k = 0; k < routeCopy.size() - 1; k++)
                {
                    vector<int>::iterator it, it1;

                    routeCopy.insert(routeCopy.begin() + k + 1, appNode);
                    it = find(routeCopy1.begin(), routeCopy1.end(), appNode);
                    int node_Index = it - routeCopy1.begin();
                    routeCopy1.erase(remove(routeCopy1.begin(), routeCopy1.end(), appNode), routeCopy1.end());
                    cout << "\nRouteCopy after insert\n";
                    for (int m : routeCopy)
                    {
                        cout << m << "  ";
                    }
                    float newDistance = distanceCheck(routeCopy, distMatrix) + distanceCheck(routeCopy1, distMatrix);
                    int capacity = checkCapacity(routeCopy, customerdata);
                    // keeping a copy of original route
                    if (truckarr[i].capacity >= capacity)
                    {
                        if (newDistance <= oldDistance)
                        {
                            cout << "\nCHECK CHECK CHECK\n";
                            route[i] = routeCopy;
                            route[rtNo] = routeCopy1;
                            cout << "\nRouteCopy after IF condition\n";
                            for (int m : routeCopy)
                            {
                                cout << m << "  ";
                            }
                            cout << "\nNew Distance before insertion : " << oldDistance << endl;
                        }
                    }
                    routeCopy1.insert(routeCopy1.begin() + node_Index, appNode);
                    routeCopy.erase(remove(routeCopy.begin(), routeCopy.end(), appNode), routeCopy.end());
                }
            }
            eachRouteOfDistMatrix.clear();
        }
    }
    for (int i = 0; i < route.size(); i++)
    {

        s1.routeDistance[i] = distanceCheck(route[i], distMatrix);
    }

    float objvalue = objfunc(customerdata, distMatrix, truckarr, vehicle, s1.routeDistance, s1.ext_transportcost);
    s2.ext_transportcost = s1.ext_transportcost;
    s2.initcost = objvalue;
    s2.notvisited = s1.notvisited;
    s2.remcapacityofvehicle = s1.remcapacityofvehicle;
    s2.routeDistance = s1.routeDistance;
    s2.vehicleroute = route;
    s2.visited = s1.visited;
    return s2;
}
/*
Ship with the internal fleet
Cost 	Vehicle no.1
239,33	0-38-9-30-34-50-16-21-29-2-11-0
Cost 	Vehicle no.2
239,25	0-12-37-44-15-45-33-39-10-49-5-46-0
Cost 	Vehicle no.3
301,97	0-32-1-22-20-35-36-3-28-31-8-26-7-48-0
Cost 	Vehicle no.4
326,15	0-4-17-42-19-40-41-13-25-14-24-43-23-6-0
Ship by external transporter
Cost 	Customer
41	18
19	27
25	47
Total cost: 1191,7
*/
