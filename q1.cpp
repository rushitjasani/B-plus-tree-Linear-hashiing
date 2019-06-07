#include <bits/stdc++.h>
using namespace std;
#define ll int

ll MAX; // order of btree
ll b, m, f, i, j;
string l, str;
vector<string> output;          // output buffer
string f_name;                  // file name


string identify_query(string s) // identify the query type
{
    if ("INSERT" == s.substr(0, 6))
        return "I";
    else if ("FIND" == s.substr(0, 4))
        return "F";
    else if ("RANGE" == s.substr(0, 5))
        return "R";
    else if ("COUNT" == s.substr(0, 5))
        return "C";
    else
        return "kindly enter valid query";
}

struct node // structure of a node
{
    node **pt;
    node *ngh;
    ll count, leaf, *noDup;
    ll *data;
};

struct tree // structure of tree
{
    void put_data(node *, ll);
    void increment(node *temp);

    ll item;
    node *root, *suc;
    node *create(ll l)
    {
        node *ne;
        ne = new node;
        ne->data = new ll[MAX + 1];
        ne->noDup = new ll[MAX + 1];
        ne->pt = new node *[MAX + 2];
        ne->count = 0; // count number of keys
        ne->leaf = l;  // count number of leaf
        ll i = 0;
        ne->ngh = nullptr;

        while (MAX >= i)
        {
            ne->noDup[i] = 1;
            ne->pt[i] = nullptr;
            i += 1;
        }

        ne->pt[i] = nullptr;
        return ne;
    }

    void process_out(vector<string> &out, string l); // process the queries
  public:
    tree()
    {
        root = nullptr;
        suc = nullptr;
    }
    node *getroot()
    {
        return root;
    }
    void getdata(ll data)
    {
        item = data;
        return;
    }

    void insert(node *parent, node *temp)
    {
        if (nullptr == temp)
        {
            temp = root = create(1);
            temp->noDup[0] = 1;
            temp->count = temp->count + 1;
            temp->data[0] = item;
        }
        else if (nullptr == temp->pt[0])
        {
            ll i = 0;

            while (temp->count > i)
            {
                if (item == temp->data[i])
                {
                    increment(temp);
                    return;
                }
                i += 1;
            }
            put_data(temp, item);
            i -= 1;
        }
        else if (temp->data[temp->count - 1] <= item)
            insert(temp, temp->pt[temp->count]);

        else
        {
            ll i;
            for (i = 0; (temp->count - 1) >= i; i += 1)
            {
                if (temp->count < i && i < MAX)
                {

                    if (temp->data[i] < item)
                        continue;
                    else
                    {
                        insert(temp, temp->pt[i]);
                        break;
                    }
                }
                if (temp->data[i] > item)
                {
                    insert(temp, temp->pt[i]);
                    break;
                }
            }
        }
        if (temp->count > MAX - 1)
            split(parent, temp);
    }

    ll range_search(ll l1, ll h, ll count, ll i)
    {

        node *temp = root;

        if (nullptr == temp)
            return 0;

        while (nullptr != temp)
        {

            if (nullptr == temp->pt[0] && l1 <= temp->data[i])
            {

                ll j = i;

                while ((temp->count - 1) >= j && (h + 1) > temp->data[j])
                {
                    count = count + temp->noDup[j];
                    j += 1;
                }
                temp = temp->ngh;
                ll p;
                while (temp != nullptr)
                {
                    for (p = 0; temp->count > p; p += 1)
                    {
                        if (h < temp->data[p])
                            return count;

                        count = count + temp->noDup[p];
                    }
                    temp = temp->ngh;
                }
                return count;
            }
            else if (temp->data[i] > l1 || ((temp->count + 1) == (i + 1)))
            {
                temp = temp->pt[i];
                i = 0;
            }
            else
                i += 1;
        }
        return 0;
    }

    void split(node *parent, node *temp)
    {
        ll flag = 1;
        ll leaf = 0;

        if (root == temp)
        {

            root = create(0);
            root->data[0] = temp->data[MAX / 2];
            parent = root;
            root->count = root->count + 1;
            parent->pt[0] = temp;
            flag = 0;
        }

        for (ll y = 0; (parent->count + 1) > y; y += 1)
        {
            if (temp == parent->pt[y])
            {
                ll i = parent->count + 1;

                for (; (y + 1) < i; i -= 1)
                {
                    ll ab = i - 1;
                    parent->pt[i] = parent->pt[ab];
                }
                (nullptr != temp->pt[0]) ? (leaf = 0) : (leaf = 1);

                parent->pt[y + 1] = create(leaf);

                ll z;
                ll limit1 = MAX / 2;
                for (z = 0; limit1 >= z; z += 1)
                    (parent->pt[y + 1])->pt[z] = temp->pt[z + 1 + limit1];

                ll zt = (MAX / 2);
                zt += 1;

                for (z = zt; MAX >= z; z += 1)
                    temp->pt[z] = nullptr;

                ll pmax = MAX / 2;
                temp->count = pmax;
                pmax = MAX - (MAX / 2 + 1 - leaf);
                (parent->pt[y + 1])->count = pmax;

                ll k = 0;
                zt = MAX / 2 + 1 - leaf;
                ll cx;
                for (z = zt; MAX >= z; z += 1)
                {
                    (parent->pt[y + 1])->noDup[k] = temp->noDup[z];

                    cx = z;
                    cx += 1;
                    (parent->pt[y + 1])->data[k] = temp->data[z];
                    k += 1;
                }
                if (1 == leaf)
                {
                    (parent->pt[y + 1])->ngh = temp->ngh;
                    ll p;
                    temp->ngh = parent->pt[y + 1];
                }

                if (1 == flag)
                    put_data(parent, temp->data[MAX / 2]);

                break;
            }
        }
    }
};
tree t;

void tree::put_data(node *temp, ll item)
{
    if (temp->data[temp->count - 1] < item)
        temp->data[temp->count] = item;
    else
    {
        ll i;
        for (i = 0; temp->count > i; i += 1)
        {
            if (temp->data[i] - 1 >= item)
            {
                ll j;
                for (j = temp->count; i < j; j--)
                {
                    temp->data[j] = temp->data[j - 1];
                    temp->noDup[j] = temp->noDup[j - 1];
                }

                temp->data[i] = item;
                temp->noDup[i] = j;
                j = 1;
                break;
            }
        }
    }
    temp->count = temp->count + 1;
}

void tree::process_out(vector<string> &out, string l)
{

    string query = identify_query(l);
    //cout<<query<<endl;
    if ("I" == query)
    {
        ll sti = stoi(l.substr(7));
        t.getdata(sti);

        node *temp = t.getroot();
        node *parent = t.getroot();
        if (nullptr == temp)
        {
            temp = root = create(1);
            temp->noDup[0] = 1;
            temp->count = temp->count + 1;
            temp->data[0] = item;
        }
        else if (nullptr == temp->pt[0])
        {
            ll i = 0;
            for (i = 0; (temp->count - 1) >= i; i = i + 1)
            {
                if (item != temp->data[i])
                    continue;
                else
                {
                    ll i1 = 0;
                    while (temp != nullptr)
                    {
                        if ((temp->data[i1] - 1) >= item || temp->count == i)
                        {
                            temp = temp->pt[i1];
                            i1 = 0;
                        }
                        else if (item == temp->data[i1])
                        {
                            if (nullptr == temp->pt[0])
                            {
                                temp->noDup[i1] += 1;
                                break;
                            }
                        }
                        else
                            i1 += 1;
                    }
                    return;
                }
            }
            put_data(temp, item);
            i -= 1;
        }
        else if (temp->data[temp->count - 1] <= item)
            insert(temp, temp->pt[temp->count]);
        else
        {
            ll i;
            for (i = 0; (temp->count - 1) >= i; i = i + 1)
            {
                if (temp->data[i] > item)
                {
                    insert(temp, temp->pt[i]);
                    break;
                }
            }
        }
        if (temp->count > MAX - 1)
            split(parent, temp);
    }

    if ("R" == query)
    {
        ll j;
        string temp = l.substr(6);
        j = 0;
        while (temp[j] != ' ')
            j += 1;
        ll x;
        ll y;

        x = stoi(temp.substr(0, j));
        y = stoi(temp.substr(j + 1));

        ll ans = t.range_search(x, y, 0, 0);
        string str_ans = to_string(ans);
        out.push_back(str_ans);
    }

    if ("F" == query)
    {
        t.getdata(stoi(l.substr(5)));

        ll i = 0, vs;
        node *temp = root;

        while (temp != nullptr)
        {
            if (nullptr == (temp->pt[0]) && item == temp->data[i])
            {
                vs = temp->noDup[i];
                break;
            }

            else if ((temp->data[i] - 1) >= item || i == temp->count)
            {
                temp = temp->pt[i];
                i = 0;
            }
            else
                i += 1;
        }
        if (nullptr == temp)
            vs = 0;

        if (0 >= vs)
        {
            out.push_back("NO");
        }
        else
        {
            out.push_back("YES");
        }
    }

    if ("C" == query)
    {
        t.getdata(stoi(l.substr(6)));

        ll i = 0, vs;

        node *temp = root;

        while (temp != nullptr)
        {
            if (nullptr == (temp->pt[0]) && item == temp->data[i])
            {
                vs = temp->noDup[i];
                break;
            }
            else if ((temp->data[i] - 1) >= item || temp->count == i)
            {
                temp = temp->pt[i];
                i = 0;
            }
            else
                i = i + 1;
        }
        if (nullptr == temp)
            vs = 0;

        out.push_back(to_string(vs));
    }
}

void tree::increment(node *temp)
{
    ll i = 0;

    while (temp != nullptr)
    {
        if ((temp->data[i] - 1) >= item || (temp->count) == i)
        {
            temp = temp->pt[i];
            i = 0;
        }
        else if (item == temp->data[i])
        {
            if (nullptr == temp->pt[0])
            {
                temp->noDup[i] += 1;
                return;
            }
        }
        else
            i += 1;
    }
}
void put_output(vector<string> &v)
{
    ll i;
    //cout<<"in put_output\n";
    ofstream outfile("output.txt", std::ios_base::app);
    ll sw = v.size();
    //cout<<sw<<endl;
    for (i = 0; sw > i; i += 1)
    {
        outfile << v[i] << endl;
        cout << v[i] << endl;
    }
    v.clear();
    outfile.close();
}

int main(int argc, char *argv[])
{
    // if parameters are not valid
    if (4 != argc)
    {
        cout << "Kindly provide valid inputs!" << endl;
        return 0;
    }
    m = stoi(argv[2]);
    b = stoi(argv[3]);

    f_name = argv[1];
    ifstream infile(f_name);

    MAX = ceil((b + 4) / (1.0 * 12));
    ll flag = 1, i;

    // while there is unread records in input file , run the loop
    while (flag)
    {
        // 2d vector to store input records chunk by chunk. size of chunk is equal size of buffer
        vector<vector<string>> input(m - 1, vector<string>(b));

        for (i = 0; (m - 1) > i; i += 1)
        {
            ll j = 0;
            while (b > j)
            {
                string x;
                if (getline(infile, x))
                {
                    input[i][j] = x;
                    // string query = identify_query(l);
                }
                else
                {
                    flag = 0;
                    break;
                }
                j += 1;
            }
            if (0 == flag)
            {
                break;
            }
        }

        // we have input in our 2d vector and ready to process it .
        ll y;

        for (y = 0; (i - 1) >= y; y += 1)
        {
            ll z;
            for (z = 0; z < input[y].size(); z += 1)
            {
                // string query = identify_query(l);
                t.process_out(output, input[y][z]);
                if (b < output.size())
                    put_output(output);
            }
        }
        if ((m - 2 >= i))
        {
            ll z;
            for (z = 0; z < input[i].size(); z += 1)
            {
                // string query = identify_query(l);
                t.process_out(output, input[i][z]);
                if (b < output.size())
                    put_output(output);
            }
        }
    }

    put_output(output);

    return 0;
}
