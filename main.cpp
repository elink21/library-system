#include <bits/stdc++.h>
using namespace std;

string join(string);
string split(string);

struct book{
	int key;
	string title;
	string abstract;
	string img;
	int stock;
};

struct user{
	string name;
	string nick;
	string pass;
	string img;
	vector<book> taken;
};
class library
{
public:

	bool request(int key)
	{
		bool successful;
		for(int i=0;i<collection.size();i++)
		{
			if(collection[i].key==key && collection[i].stock)
			{
				active.taken.push_back(collection[i]);
				collection[i].stock--;
				successful=true;
				updateCollection();
				break;
			}
			else if(collection[i].key==key && !collection[i].stock)
			{
				cout<<"\nLo sentimos no hay libros en inventario\n";
				return false;
			}
		}

		if(successful)
		{
			for(int i=0;i<users.size();i++)
			{
				if(users[i].nick==active.nick&&users[i].pass==active.pass)
				{
					users[i]=active;
					updateUsers();
					return true;
				}
			}
		}

		return false;
	}

	bool giveBack(int key)
	{
		for(int i=0;i<active.taken.size();i++)
		{
			if(active.taken[i].key==key)
			{
				active.taken.erase(active.taken.begin()+i);
				for(int i=0;i<users.size();i++)
				{
					if(users[i].nick==active.nick&&users[i].pass==active.pass)
					{
						users[i]=active;
						updateUsers();
					}
				}
				break;
			}
		}
		for(int i=0;i<collection.size();i++)
		{
			if(collection[i].key==key)
			{
				collection[i].stock++;
				updateCollection();
				return true;
			}
		}
		return false;
	}

	bool login()
	{
		user attempt;
		cout<<"Ingrese usuario: ";
		cin>>attempt.nick;
		cout<<"Ingrese password: ";
		cin>>attempt.pass;
		for(user u:this->users)
		{
			if(u.nick==attempt.nick&&u.pass==attempt.pass)
			{
				cout<<"\nUsuario logeado!\n";
				cout<<"Bienvenido: "<<split(u.name)<<"\n";
				this->active=u;
				return true;
			}
		}
		return false;
	}

	bool logout()
	{
		user emp;
		active=emp;
	}

	vector<book> getCollection()
	{
		return this->collection;
	}

	vector<user> getUsers()
	{
		return this->users;
	}

	void readCollection()
	{
		vector<book>temp;
		book t;
		ifstream f("db/collection.txt",ios::in);

		while(f>>t.key>>t.title>>t.abstract>>t.img>>t.stock)
		temp.push_back(t);

		this->collection=temp;
	}

	void readUsers()
	{
		user u;
		ifstream f("db/users.txt",ios::in);
		string bookSaved;
		while(f>>u.name>>u.nick>>u.pass>>u.img>>bookSaved)
		{
			istringstream iss(split(bookSaved));
			vector<string> keys{istream_iterator<string>{iss},istream_iterator <string> {}};
			for (string s:keys)
			{
				for(book b: this->collection)
				{
					if(atoi(s.c_str())==b.key)
					u.taken.push_back(b);
				}
			}
			users.push_back(u);
			u.taken={};
		}
	}

	void newBook()
	{
		book newBook;
		int countRepeated=0;
		do {
			cin.get();
			countRepeated=0;
			cout<<"Cual es el titulo del nuevo libro?: ";
			getline(cin,newBook.title);
			cout<<"Cual es su clave unica?: ";
			cin>>newBook.key;
			for(book b:this->collection)
			{
				if(newBook.title==b.title || newBook.key==b.key) countRepeated+=1;
			}
		} while(countRepeated);
		cout<<"Describa un resumen: ";
		cin.get();
		getline(cin,newBook.abstract);
		cout<<"Como se llama la imagen en img?: ";
		cin>>newBook.img;
		cout<<"Finalmente, cuantos libros se registran?: ";
		cin>>newBook.stock;
		collection.push_back(newBook);
		cin.get();
		updateCollection();
	}


	void newUser()
	{
		user me;
		int countRepeated=0;
		string validate;
		cin.get();
		cout<<"Ingrese su nombre: ";
		getline(cin,me.name);
		do
		{
			countRepeated=0;
			cout<<"Ingrese su nombre de usuario: ";
			cin>>me.nick;
			for(user u:users)
			{
				if (u.nick==me.nick)countRepeated+=1;
			}
			if(countRepeated) cout<<"\nLo sentimos este nick ya esta ocupado";
		} while(countRepeated);
		cout<<"Ingrese su password: ";
		cin>>me.pass;
		while(validate!=me.pass)
		{
			cout<<"Ingrese la primer password de nuevo: ";
			cin>>validate;
		}
		cout<<"Ingrese el nombre de su imagen: ";
		cin>>me.img;
		book null;
		null.key=0;
		me.taken.push_back(null);
		users.push_back(me);
		updateUsers();
	}

	void showCollection()
	{
		makeCollectionPage();
		system("collection.html");
	}

	void showUserPage()
	{
		makeUserPage();
		system("user.html");
	}

	void destroyUser(string nick)
	{
		int index=-1;
		for(int i=0;i<this->users.size();i++)
			if(this->users[i].nick==nick) index=i;
		if(index!=-1){ this->users.erase(this->users.begin()+index);}
		else cout<<"\nUsuario no encontrado\n";
		updateUsers();
	}
	void destroyBook(int key)
	{
		int index=-1;
		for(int i=0;i<this->collection.size();i++)
			if(this->collection[i].key==key) index=i;
		if(index!=-1){this->collection.erase(this->collection.begin()+index);}
		else cout<<"\nLibro no encontrado\n";
		updateCollection();
	}
	void editBook(int key)
	{
		cin.get();
		int index=-1;
		for(int i=0;i<this->collection.size();i++)
			if(this->collection[i].key==key) index=i;
		if(index!=-1)
		{
			cout<<"Inserte el nuevo nombre del libro: ";
			getline(cin,this->collection[index].title);
			cout<<"Ahora ingrese el nuevo resumen: ";
			getline(cin,this->collection[index].abstract);
			cout<<"Ingrese el nombre de la nueva imagen: ";
			cin>>this->collection[index].img;
			cout<<"Finalmente actualize el stock: ";
			cin>>this->collection[index].stock;
			updateCollection();
		} else cout<<"\nLibro no encontrado\n";
	}
	void editUser(string nick)
	{
		cin.get();
		int index=-1;
		for(int i=0;i<this->users.size();i++)
			if(this->users[i].nick==nick) index=i;
		if(index!=-1)
		{
			cout<<"Inserte el nuevo nombre del usuario: ";
			getline(cin,this->users[index].name);
			cout<<"Ahora ingrese el nuevo password: ";
			cin>>this->users[index].pass;
			cout<<"Ingrese el nombre de la nueva imagen: ";
			cin>>this->users[index].img;
			updateUsers();
		} else cout<<"\nUsuario no encontrado\n";
	}
	void sortBooksByKey()
	{
		for(int i=0;i<this->collection.size();i++)
		{
			for(int j=0;j<this->collection.size();j++)
			{
				if(this->collection[i].key<this->collection[j].key)
				{
					book temp;
					temp=this->collection[i];
					this->collection[i]=this->collection[j];
					this->collection[j]=temp;
				}
			}
		}
		updateCollection();
	}
	void sortUsersByNick()
	{
		for(int i=0;i<this->users.size();i++)
		{
			for(int j=0;j<this->users.size();j++)
			{

				if(strcmp(this->users[i].nick.c_str(),this->users[j].nick.c_str())<1)
				{
					user temp;
					temp=this->users[i];
					this->users[i]=this->users[j];
					this->users[j]=temp;
				}
			}
		}
		updateUsers();
	}
	void showAdminPage()
	{
		makeAdminPage();
		system("admin.html");
	}

private:
	vector<book> collection;
	vector<user> users;
	user active;
protected:
	void makeAdminPage()
	{
		ofstream out("admin.html",ios::out);
		out<<"<!DOCTYPE html>\n";
		out<<"<html lang='en'>\n";
		out<<"<head>\n";
		out<<"<meta charset='UTF-8'>\n";
		out<<"<meta name='viewport' content='width=device-width,";
		out<<"user-scalable=no, initial-scale=1.0,maximum-scale=1.0, minimum-scale=1.0'>\n";
		out<<"<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'";
		out<<"integrity='sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u'\n";
		out<<"crossorigin='anonymous'>\n";

		out<<"<title>Pagina de administrador</title>\n";
		out<<"</head>\n";
		out<<"<body>\n";
		out<<"<div class='container col-md-12'>\n";
		out<<"<h1>Coleccion de libros disponibles</h1>\n";
		out<<"<table class='table'>\n";
		out<<"<tr>\n";
		out<<"<th>Clave</th>\n";
		out<<"<th>Titulo</th>\n";
		out<<"<th>Resumen</th>\n";
		out<<"<th>Imagen</th>\n";
		out<<"<th>Stock</th>\n";
		out<<"</tr>\n";
		for(book b:collection)
		{
			out<<"<tr>\n";
			out<<"<td>"<<b.key<<"</td>\n";
			out<<"<td>"<<split(b.title)<<"</td>\n";
			out<<"<td>"<<split(b.abstract)<<"</td>\n";
			out<<"<td><img class=img-responsive width=100px height=100px src='images/"<<b.img<<"'><td>\n";
			out<<"<td>"<<b.stock<<"</td>\n";
			out<<"</tr>\n";
		}

		out<<"</table>\n";
		out<<"<h1>Usuarios registrados</h1>\n";
		out<<"<table class='table'>\n";
		out<<"<tr>\n";
		out<<"<th>Nombre</th>\n";
		out<<"<th>Nick</th>\n";
		out<<"<th>Password</th>\n";
		out<<"<th>Imagen</th>\n";
		out<<"</tr>\n";
		for(user u:users)
		{
			out<<"<tr>\n";
			out<<"<td>"<<split(u.name)<<"</td>\n";
			out<<"<td>"<<u.nick<<"</td>\n";
			out<<"<td>"<<u.pass<<"</td>\n";
			out<<"<td><img class=img-responsive width=100px height=100px src='images/"<<u.img<<"'><td>\n";
			out<<"</tr>\n";
		}

		out<<"</table>\n";
		out<<"</div>\n";
		out<<"<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'\n";
		out<<"integrity='sha384-Tc5IQib027qvyjSMfHjOMaLkf\n";
		out<<"uWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa '\n";
		out<<"crossorigin='anonymous'>\n";
		out<<"</script>\n";
		out<<"</body>\n";
		out<<"</html>\n";
	}
	void makeUserPage()
	{
		fstream out("user.html",ios::out);
		out<<"<!DOCTYPE html>\n";
		out<<"<html lang='en'>\n";
		out<<"<head>\n";
		out<<"<meta charset='UTF-8'>\n";
		out<<"<meta name='viewport' content='width=device-width,";
		out<<"user-scalable=no, initial-scale=1.0,maximum-scale=1.0, minimum-scale=1.0'>\n";
		out<<"<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'";
		out<<"integrity='sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u'\n";
		out<<"crossorigin='anonymous'>\n";

		out<<"<title>Pagina de "<<active.name<<" </title>\n";
		out<<"</head>\n";
		out<<"<body>\n";
		out<<"<div class='container jumbotron col-md-12'>\n";
		out<<"<h1>Bienvenido "<<active.name<<"!</h1>\n";
		out<<"<img class=img-responsive width=100px height=100px src='images/"<<active.img<<"'>\n";
		out<<"</div>\n";
		out<<"<h1>Estos son los libros que posees</h1>\n";
		out<<"<table class='table'>\n";
		out<<"<tr>\n";
		out<<"<th>Clave</th>\n";
		out<<"<th>Titulo</th>\n";
		out<<"<th>Resumen</th>\n";
		out<<"<th>Imagen</th>\n";
		out<<"</tr>\n";
		for(book b:active.taken)
		{
			if(b.key!=0){
				out<<"<tr>\n";
				out<<"<td>"<<b.key<<"</td>\n";
				out<<"<td>"<<split(b.title)<<"</td>\n";
				out<<"<td>"<<split(b.abstract)<<"</td>\n";
				out<<"<td><img class=img-responsive width=300px height=300px src='images/"<<b.img<<"'><td>\n";
				out<<"</tr>\n";}
			}
			out<<"</table>\n";
			out<<"</div>\n";
			out<<"<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'\n";
			out<<"integrity='sha384-Tc5IQib027qvyjSMfHjOMaLkf\n";
			out<<"uWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa '\n";
			out<<"crossorigin='anonymous'>\n";
			out<<"</script>\n";
			out<<"</body>\n";
			out<<"</html>\n";

		}

		void makeCollectionPage()
		{
			ofstream out("collection.html",ios::out);
			out<<"<!DOCTYPE html>\n";
			out<<"<html lang='en'>\n";
			out<<"<head>\n";
			out<<"<meta charset='UTF-8'>\n";
			out<<"<meta name='viewport' content='width=device-width,";
			out<<"user-scalable=no, initial-scale=1.0,maximum-scale=1.0, minimum-scale=1.0'>\n";
			out<<"<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'";
			out<<"integrity='sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u'\n";
			out<<"crossorigin='anonymous'>\n";

			out<<"<title>Coleccion de titulos</title>\n";
			out<<"</head>\n";
			out<<"<body>\n";
			out<<"<div class='container col-md-12'>\n";
			out<<"<h1>Coleccion de libros disponibles</h1>\n";
			out<<"<table class='table'>\n";
			out<<"<tr>\n";
			out<<"<th>Clave</th>\n";
			out<<"<th>Titulo</th>\n";
			out<<"<th>Resumen</th>\n";
			out<<"<th>Imagen</th>\n";
			out<<"<th>Stock</th>\n";
			out<<"</tr>\n";
			for(book b:collection)
			{
				out<<"<tr>\n";
				out<<"<td>"<<b.key<<"</td>\n";
				out<<"<td>"<<split(b.title)<<"</td>\n";
				out<<"<td>"<<split(b.abstract)<<"</td>\n";
				out<<"<td><img class=img-responsive width=100px height=100px src='images/"<<b.img<<"'><td>\n";
				out<<"<td>"<<b.stock<<"</td>\n";
				out<<"</tr>\n";
			}

			out<<"</table>\n";
			out<<"<h1>Usuarios registrados</h1>\n";
			out<<"<ul>\n";
			for(user u:users)
			{
				out<<"<li> <strong>"<<u.name<<"</strong></li>\n";
			}
			out<<"</ul>\n";
			out<<"</div>\n";
			out<<"<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'\n";
			out<<"integrity='sha384-Tc5IQib027qvyjSMfHjOMaLkf\n";
			out<<"uWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa '\n";
			out<<"crossorigin='anonymous'>\n";
			out<<"</script>\n";
			out<<"</body>\n";
			out<<"</html>\n";

		}

		void updateCollection()
		{
			ofstream o("db/collection.txt",ios::out);
			for(book b: this->collection)
			{
				o<<b.key<<" "<<join(b.title)<<" "<<join(b.abstract)<<" "<<b.img<<" "<<b.stock<<"\n";
			}
			cout<<"After\n";
		}

		void updateUsers()
		{
			ofstream o("db/users.txt", ios::out);
			string bookSaved="";
			for(user u:this->users)
			{
				bookSaved="";
				for(book b: u.taken)
				{
					stringstream s;
					s<<b.key;
					bookSaved+=s.str()+" ";
				}
				if(bookSaved=="")bookSaved="0";
				o<<join(u.name)<<" "<<u.nick<<" "<<u.pass<<" "<<u.img<<" "<<join(bookSaved)<<"\n";
			}

		}

	};


	int main()
	{
		library pearson;
		pearson.readCollection();
		pearson.readUsers();
		int option=0,subOption=0,k=0,r=0;
		while(option!=4)
		{
			cout<<"\nSistema de prestamo de libros\n";
			cout<<"\n1.-Registrarse   2.- Ingresar 3.-Administrador 4.-Salir\n";
			cout<<"Ingrese: ";
			cin>>option;
			switch(option)
			{
				case 1:
				pearson.newUser();
				break;
				case 2:
				if(pearson.login())
				{
					subOption=0;
					while(subOption!=5)
					{
						cout<<"Opciones de cuenta: \n";
						cout<<"1.-Pedir libro por clave\n"
						<<"2.-Regresar libro con clave\n"
						<<"3.-Mostrar pagina de catalogo\n"
						<<"4.-Mostrar mi pagina de usuario\n"
						<<"5.-Salir de la cuenta\n\n";
						cout<<"Ingrese: ";
						cin>>subOption;
						switch(subOption)
						{
							case 1:
							pearson.showCollection();
							cout<<"Se acaba de abrir la pagina,inserte la clave del libro deseado: ";
							cin>>k;
							if(pearson.request(k))
							{
								cout<<"\nAhora ya tienes el libro, puedes revisarlo en tu pagina\n";
							}
							else cout<<"\nError al obtener el libro\n";
							break;
							case 2:
							cout<<"Inserte la clave del libro a devolver: ";
							cin>>r;
							pearson.giveBack(r);
							cout<<"\nPuedes comprobar la devolucion del libro en tu pagina de usuario\n";
							break;
							case 3:
							pearson.showCollection();
							break;
							case 4:
							pearson.showUserPage();
							break;
						}

					}
				}
				break;
				case 3:
				string admin;
				cout<<"\nIngrese password: ";
				cin>>admin;
				if(admin=="admin")
				{
					int adminOption;
					pearson.showAdminPage();
					cout<<"\nOpciones de administracion\n";
					cout<<"1.-Eliminar usuario por nick\n"
					<<"2.-Eliminar libro por clave\n"
					<<"3.-Editar libro por clave\n"
					<<"4.-Editar usuario por nick\n"
					<<"5.-Ordenar libros por clave\n"
					<<"6.-Ordenar usuarios por nick\n";
					cout<<"Ingrese la opcion de administracion: ";
					cin>>adminOption;
					int key;
					string nick;
					switch (adminOption) {
						case 1:
							cout<<"Ingrese nick: ";
							cin>>nick;
							pearson.destroyUser(nick);
						break;
						case 2:
							cout<<"Ingrese clave: ";
							cin>>key;
							pearson.destroyBook(key);
						break;
						case 3:
							cout<<"Ingrese clave: ";
							cin>>key;
							pearson.editBook(key);
						break;
						case 4:
							cout<<"Ingrese nick: ";
							cin>>nick;
							pearson.editUser(nick);
						break;
						case 5:
							pearson.sortBooksByKey();
						break;
						case 6:
							pearson.sortUsersByNick();
						break;
						case 7:
							pearson.newBook();
						break;
					}
				}
			}
		}

	}

	//just helpers
	string join(string s)
	{
		for(int i=0;i<s.size();i++)
		{
			if(s[i]==' ')s[i]='-';
		}
		return s;
	}

	string split(string s)
	{
		for(int i=0;i< s.size();i++)
		{
			if(s[i]=='-')s[i]=' ';
		}
		return s;
	}
