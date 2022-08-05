#include <iostream>
#include "function.h"
using namespace std;
// using namespace std::chrono;

int main(int argc, char *argv[])
{
    VRPPC p;
    string filename = argv[1];
    p.takeInputFile(filename);
    p.showCustomerData();
    p.showVehicleData();
    p.showDistanceMatrix();
    p.showTruckDetails();

    vector<vector<float>> distance = p.getDistanceMatrix();

    truck *TruckArray = p.getTruckArr();

    sol s1, best_found_sol;
    vector<vector<int>> customerdata = p.getCustomerData();
    s1 = p.initialsol(customerdata, distance, TruckArray);

    cout << "\nObjective value of Initial solution: " << s1.initcost;

    best_found_sol = s1;
    int flag = 0;
    int count = 0, count1 = 0;

    int itr = 0;
    while (count <= 50)
    {
        itr++;
        float randMultiplier = 1.01 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.05 - 1.01)));
        if (count1 > 50)
        {
            randMultiplier = 1;
        }
        flag = 0;
        s1 = p.newsol(customerdata, distance, TruckArray, s1, randMultiplier);

        cout << "\nTwo Opt Routes : \n";
        for (int i = 0; i < s1.vehicleroute.size(); i++)
        {
            for (int j = 0; j < s1.vehicleroute[i].size(); j++)
            {
                cout << s1.vehicleroute[i][j] << " ";
            }
            cout << endl;
        }
        cout << "\nObjective value : " << s1.initcost;

        if (s1.initcost < best_found_sol.initcost)
        {
            best_found_sol = s1;
            flag = 1;
            count1++;
        }
        s1 = p.adjacentSwapping(customerdata, distance, TruckArray, s1.ext_transportcost, s1, randMultiplier);

        cout << "\nadjacent Swapping Routes : \n";
        for (int i = 0; i < s1.vehicleroute.size(); i++)
        {
            for (int j = 0; j < s1.vehicleroute[i].size(); j++)
            {
                cout << s1.vehicleroute[i][j] << " ";
            }
            cout << endl;
        }
        cout << "\nObjective value : " << s1.initcost;

        if (s1.initcost < best_found_sol.initcost)
        {
            best_found_sol = s1;
            flag = 1;
            count1++;
        }
        s1 = p.one_oneSwapping(customerdata, distance, TruckArray, s1.ext_transportcost, s1, randMultiplier);

        cout << "\none_one Swapping Routes : \n";
        for (int i = 0; i < s1.vehicleroute.size(); i++)
        {
            for (int j = 0; j < s1.vehicleroute[i].size(); j++)
            {
                cout << s1.vehicleroute[i][j] << " ";
            }
            cout << endl;
        }
        cout << "\nObjective value : " << s1.initcost;

        if (s1.initcost < best_found_sol.initcost)
        {
            best_found_sol = s1;
            flag = 1;
            count1++;
        }
        s1 = p.singleInsertionSwapping(customerdata, distance, TruckArray, s1.ext_transportcost, s1, randMultiplier);

        cout << "\nsingle Insertion Swapping Routes : \n";
        for (int i = 0; i < s1.vehicleroute.size(); i++)
        {
            for (int j = 0; j < s1.vehicleroute[i].size(); j++)
            {
                cout << s1.vehicleroute[i][j] << " ";
            }
            cout << endl;
        }
        cout << "\nObjective value : " << s1.initcost;

        if (s1.initcost < best_found_sol.initcost)
        {
            best_found_sol = s1;
            flag = 1;
            count1++;
        }

        s1 = p.insertWithMaxLink(customerdata, distance, TruckArray, s1, randMultiplier);
        cout << "\nInsert with max link routes : \n";
        for (int i = 0; i < s1.vehicleroute.size(); i++)
        {
            for (int j = 0; j < s1.vehicleroute[i].size(); j++)
            {
                cout << s1.vehicleroute[i][j] << " ";
            }
            cout << endl;
        }
        cout << "\nObjective value : " << s1.initcost;

        if (s1.initcost < best_found_sol.initcost)
        {
            best_found_sol = s1;
            flag = 1;
            count1++;
        }
        s1 = p.externalNodeInsertion(customerdata, distance, TruckArray, s1.ext_transportcost, s1, randMultiplier);
        cout << "\nexternal Node Insertion Routes : \n";
        for (int i = 0; i < s1.vehicleroute.size(); i++)
        {
            for (int j = 0; j < s1.vehicleroute[i].size(); j++)
            {
                cout << s1.vehicleroute[i][j] << " ";
            }
            cout << endl;
        }
        cout << "\nObjective value : " << s1.initcost;

        if (s1.initcost < best_found_sol.initcost)
        {
            best_found_sol = s1;
            flag = 1;
            count1++;
        }
        s1 = p.externalNodeSwapping(customerdata, distance, TruckArray, s1.ext_transportcost, s1, randMultiplier);
        cout << "\nexternal Node Swapping Routes : \n";
        for (int i = 0; i < s1.vehicleroute.size(); i++)
        {
            for (int j = 0; j < s1.vehicleroute[i].size(); j++)
            {
                cout << s1.vehicleroute[i][j] << " ";
            }
            cout << endl;
        }
        cout << "\nObjective value : " << s1.initcost;
        if (s1.initcost < best_found_sol.initcost)
        {
            best_found_sol = s1;
            flag = 1;
            count1++;
        }
        if (flag == 0)
        {
            count++;
        }
    }

    cout << "\nFinal Objective Value : " << best_found_sol.initcost << "    " << count1 << " no of times updated"
         << "\n";
    cout << "\nNo of times iterated " << itr << endl;
    cout << "\nFinal cuostomers served by External Fleet" << endl;
    for (int x : best_found_sol.notvisited)
    {
        cout << x << "  ";
    }
    cout << "\nFinal Routes Routes : \n";
    for (int i = 0; i < best_found_sol.vehicleroute.size(); i++)
    {
        for (int j = 0; j < best_found_sol.vehicleroute[i].size(); j++)
        {
            cout << best_found_sol.vehicleroute[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
