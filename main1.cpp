#include <iostream>
#include "function.h"
using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
    VRPPC p;
    string filename = argv[1];
    p.takeInputFile(filename);
    p.showCustomerData();
    p.showVehicleData();
    p.showDistanceMatrix();
    p.showTruckDetails();

    auto start = high_resolution_clock::now();

    // fleet f1, f2, f3, f4;

    // f1.vehicleNo = 1;
    // f1.route = {0, 48, 23, 24, 43, 7, 26, 8, 31, 28, 22, 1, 32, 0};

    // f2.vehicleNo = 2;
    // f2.route = {0, 14, 25, 13, 41, 40, 19, 42, 44, 17, 4, 0};

    // f3.vehicleNo = 3;
    // f3.route = {0, 37, 15, 45, 33, 39, 10, 30, 34, 9, 49, 38, 0};

    // f4.vehicleNo = 4;
    // f4.route = {0, 2, 3, 36, 35, 20, 29, 21, 50, 16, 11, 46, 0};

    vector<vector<float>> distance = p.getDistanceMatrix();

    truck *TruckArray = p.getTruckArr();

    sol s1;
    vector<vector<int>> customerdata = p.getCustomerData();
    s1 = p.initialsol(customerdata, distance, TruckArray);
    int improvement = 1, count = 0;
    float temp_obj = s1.initcost;
    while (improvement == 1)
    {
        float temp1 = s1.initcost;
        count = 0;
        s1 = p.newsol(customerdata, distance, TruckArray, s1);
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
        if (s1.initcost <= temp_obj)
        {
            count++;
            temp_obj = s1.initcost;
        }
        s1 = p.adjacentSwapping(customerdata, distance, TruckArray, s1.ext_transportcost, s1);
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
        if (s1.initcost <= temp_obj)
        {
            count++;
            temp_obj = s1.initcost;
        }
        s1 = p.one_oneSwapping(customerdata, distance, TruckArray, s1.ext_transportcost, s1);
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
        if (s1.initcost <= temp_obj)
        {
            count++;
            temp_obj = s1.initcost;
        }
        s1 = p.singleInsertionSwapping(customerdata, distance, TruckArray, s1.ext_transportcost, s1);
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
        if (s1.initcost <= temp_obj)
        {
            count++;
            temp_obj = s1.initcost;
        }
        s1 = p.externalNodeInsertion(customerdata, distance, TruckArray, s1.ext_transportcost, s1);
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
        if (s1.initcost <= temp_obj)
        {
            count++;
            temp_obj = s1.initcost;
        }
        s1 = p.externalNodeSwapping(customerdata, distance, TruckArray, s1.ext_transportcost, s1);
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
        if (s1.initcost <= temp_obj)
        {
            count++;
            temp_obj = s1.initcost;
        }
        if (count >= 1)
        {
            if (temp_obj == temp1)
            {
                improvement = 0;
            }
            else
            {
                improvement = 1;
            }
        }
        else
        {
            improvement = 0;
        }
    }

    // cout << "\nCount: " << count;
    // cout << "\nFinal Objective Value : " << s1.initcost << "\n";

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "\nComputation time : " << float(duration.count() / 1000) << " seconds";

    return 0;
}