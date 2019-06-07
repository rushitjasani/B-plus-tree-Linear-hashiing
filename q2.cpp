#include <bits/stdc++.h>
using namespace std;
#define ll int
#define vi vector<int>
#define si set<int>

// number of buffers and size of each buffer;
ll m, b;

// main hash_value table
vector<si> v;

// to store store output
vi  output;

// will point at the index which will get split
ll split_ptr;

// represents value in hash_value function
ll split_val;

// h(1) .denotes mod value
ll hash_value = 2;

// r value, to find the ratio
ll number_of_records = 0;
ll bucket_size;

// to store file name
char *f_name;
ifstream file_name;

// fetch value from string
ll getval(string s)
{
	ll x;
	stringstream ss(s);
	ss >> x;
	return x;
}

// find key for the value supplied
ll linear_hashing(ll input, ll size)
{
	ll x;
	x = input % hash_value;
	if (0 > input)
	{
		x = x + hash_value;
	}

	while (size < x)
	{
		ll temp = x;
		ll bt = -1;

		while (temp != 0)
		{
			bt += 1;
			// temp /= 2;
			temp = temp >> 1;
		}
		x ^= 1 << bt;
	}
	return x;
}

// send the output buffer to the file
void flush_buffer(vi  v)
{
	ofstream outfile("output.txt", ios_base::app);

	for (ll i = 0; i < output.size(); i+=1)
	{
		cout << output[i] << endl;
		outfile << output[i] << endl;
	}
	outfile.close();
	return;
}

// upon splitting all records in the bucket would get rehashed to new buckets
void rehash()
{
	si::iterator it = v[split_ptr].begin();
	vi  temp;
	for (; it != v[split_ptr].end(); it++)
	{
		ll index = linear_hashing(*it, v.size());
		if (index != split_ptr)
		{
			temp.push_back(*it);
			v[split_ptr].erase(*it);
			//v[index].insert(*it);
		}
	}

	for (ll i = 0; i < temp.size(); i+=1)
	{
		ll index = linear_hashing(temp[i], v.size());
		v[index].insert(temp[i]);
	}
	return;
}

// implements split function
void split()
{
	bucket_size = bucket_size +  (b / 4);

	si s;
	v.push_back(s);
	hash_value = 1 << (int)(ceil(log2(v.size())));

	rehash();

	split_ptr = (split_ptr + 1) % split_val;
	if (split_ptr == 0)
		split_val = split_val << 1;
	return;
}

// handles linear hashing, splitting , rehashing and overflow
void hashing(ll input)
{
	ll index, size_of_bucket;
	ll number_of_buckets = v.size() - 1;
	index = linear_hashing(input, number_of_buckets);
	size_of_bucket = v[index].size();
	v[index].insert(input);
	//cout<<input<<endl;
	if ( size_of_bucket < v[index].size() )
	{
		output.push_back(input);
		number_of_records+=1;
		//cout<<"->"<<input<<"\n";
		if (b == output.size())
		{
			flush_buffer(output);
			output.clear();
		}
	}

	//check for splitting

	float ratio = number_of_records / ((float)(bucket_size));
	if (0.75 < ratio)
	{
		split();
	}
	return;
}

ll main(ll argc, char *argv[])
{

	//	TAKE INPUT FROM COMMANDLINE
	if (argc != 4) // if parameters are not valid
	{
		cout << "Kindly provide valid inputs!";
		return 0;
	}

	f_name = argv[1];
	ifstream infile(f_name);
	// getting value of m and b from command line
	m = getval(argv[2]);
	b = getval(argv[3]);

	// INITIALIZE ALL VALUES

	cout << "\nBuffer size: " << b << endl;
	cout << "Number of Buffers: " << m << endl;
	split_ptr = 0;
	split_val = 2;
	bucket_size = (b / 4);
	si t0, t1;
	v.push_back(t0); // initial buckets
	v.push_back(t1); // initial buckets

	ll flag = 1, i;
	cout << "\n\n 			*****  OUTPUT  ***** \n";
	// while there is unread records in input file , run the loop
	while (flag != 0)
	{
		// 2d vector to store input records chunk by chunk. size of chunk is equal size of buffer

		vector<vi > input(m - 1, vi (b));
		ll j = 0;
		for (i = 0; (m-1) > i; i+=1)
		{
			j = 0;

			while (b > j)
			{
				ll x;
				if (infile >> x)
				{
					input[i][j] = x;
				}
				else
				{
					flag = 0;
					break;
				}
				j+=1;
			}
			if (0 == flag)
			{
				break;
			}
		}
		// NOW we have input in our 2d vector and ready to process it .

		ll p = 0;

		for (p = 0; p < i; p+=1)
		{
			for (ll q = 0; b > q; q+=1)
			{
				//cout<<"here"<<input[p][q]<<endl;
				hashing(input[p][q]);
			}
		}
		if ((m - 2) >= i )
		{
			ll q = 0;
			while (j > q) 
			{
				//cout<<"here"<<input[p][q]<<endl;
				hashing(input[p][q]);
				q+=1;
			}
		}
	}

	// send all remaining values in output buffer to file.
	flush_buffer(output);
	cout << "\n=============================================\nOutput is stored in output.txt\n";
	return 1;
}
