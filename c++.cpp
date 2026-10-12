#include <iostream>
#include <string>
using namespace std;

struct Flight {
    string id, type;
    int passengers, delay, urgency;
    float fuel, distance, priority;
    bool emergency;
};

struct Runway {
    int id;
    bool available;
};

struct Node {
    Flight f;
    Node *left, *right;
};

// Calculate flight priority
float getPriority(Flight f) {
    float p = f.urgency * 10;
    p += (100 - f.fuel) * 0.5;
    p += f.delay * 0.5;
    p += (100 - f.distance) * 0.1;

    if (f.emergency)
        p += 100;

    return p;
}

// Linear Search
int searchFlight(Flight a[], int n, string id) {
    for (int i = 0; i < n; i++)
        if (a[i].id == id)
            return i;

    return -1;
}

// Selection Sort - highest priority first
void sortFlights(Flight a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int max = i;

        for (int j = i + 1; j < n; j++)
            if (a[j].priority > a[max].priority)
                max = j;

        Flight temp = a[i];
        a[i] = a[max];
        a[max] = temp;
    }
}

// Create BST node
Node* createNode(Flight f) {
    Node* p = new Node;
    p->f = f;
    p->left = p->right = NULL;
    return p;
}

// Insert into BST
Node* insert(Node* root, Flight f) {
    if (root == NULL)
        return createNode(f);

    if (f.priority > root->f.priority)
        root->left = insert(root->left, f);
    else
        root->right = insert(root->right, f);

    return root;
}

// Display BST in priority order
void inorder(Node* root) {
    if (root == NULL)
        return;

    inorder(root->left);

    cout << root->f.id << "  Priority: "
         << root->f.priority << endl;

    inorder(root->right);
}

// Display flights
void display(Flight a[], int n) {
    cout << "\nID\tType\tFuel\tDelay\tUrgency\tPriority\n";

    for (int i = 0; i < n; i++)
        cout << a[i].id << "\t"
             << a[i].type << "\t"
             << a[i].fuel << "\t"
             << a[i].delay << "\t"
             << a[i].urgency << "\t"
             << a[i].priority << endl;
}

// Schedule flights
void schedule(Flight a[], int n, Runway r[], int rn) {

    sortFlights(a, n);

    cout << "\n===== RUNWAY SCHEDULE =====\n";

    int runway = 0;

    for (int i = 0; i < n; i++) {

        while (runway < rn && !r[runway].available)
            runway++;

        if (runway == rn) {
            cout << "No runway available for "
                 << a[i].id << endl;
            continue;
        }

        cout << "Flight " << a[i].id
             << " -> Runway " << r[runway].id;

        if (a[i].emergency)
            cout << " [EMERGENCY]";

        cout << endl;

        r[runway].available = false;
        runway++;
    }
}

int main() {

    Flight flights[10] = {
        {"AI101", "A320", 160, 20, 5, 45, 35, 0, false},
        {"6E202", "B737", 180, 40, 7, 25, 20, 0, false},
        {"EK303", "B777", 300, 10, 4, 60, 50, 0, false},
        {"SG404", "ATR72", 70, 55, 8, 15, 15, 0, false},
        {"UK505", "A350", 280, 5, 10, 10, 10, 0, true}
    };

    int n = 5;

    // Calculate priorities
    for (int i = 0; i < n; i++)
        flights[i].priority = getPriority(flights[i]);

    Runway runways[3] = {
        {1, true},
        {2, true},
        {3, true}
    };

    int choice;

    do {
        cout << "\n==============================\n";
        cout << " AIRPORT RUNWAY SCHEDULING\n";
        cout << "==============================\n";
        cout << "1. Display Flights\n";
        cout << "2. Search Flight\n";
        cout << "3. Sort by Priority\n";
        cout << "4. Schedule Runways\n";
        cout << "5. Display Priority BST\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {

            display(flights, n);

        } else if (choice == 2) {

            string id;
            cout << "Enter Flight ID: ";
            cin >> id;

            int pos = searchFlight(flights, n, id);

            if (pos == -1)
                cout << "Flight not found.\n";
            else
                cout << "Flight " << flights[pos].id
                     << " found. Priority = "
                     << flights[pos].priority << endl;

        } else if (choice == 3) {

            sortFlights(flights, n);
            cout << "Flights sorted by priority.\n";
            display(flights, n);

        } else if (choice == 4) {

            schedule(flights, n, runways, 3);

        } else if (choice == 5) {

            Node* root = NULL;

            for (int i = 0; i < n; i++)
                root = insert(root, flights[i]);

            cout << "\nFlights in Priority BST:\n";
            inorder(root);

        } else if (choice == 6) {

            cout << "Program ended.\n";

        } else {

            cout << "Invalid choice.\n";
        }

    } while (choice != 6);

    return 0;
}