#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>

typedef struct Koor
{
	int x; int y;
} koor;

typedef struct Blok
{
	koor prvi; koor drugi;
} blok;

typedef struct Graph
{
	blok* cvorovi;
	int **a; int x; int y;
	char** board;
	int n;
	blok pocblok;
	blok krajblok;
	blok trenblok;
} graph;

graph* g = NULL;

int EqBlok(blok b1, blok b2)
{
	if (b1.prvi.x == b2.prvi.x && b1.prvi.y == b2.prvi.y && b1.drugi.x == b2.drugi.x && b1.drugi.y == b2.drugi.y) return 1;
	else return 0;
}

//jedan pada u dva

blok levi_jedan(blok b1)
{
	blok b2;
	b2.prvi.x = b1.prvi.x;
	b2.prvi.y = b1.prvi.y - 2;
	b2.drugi.x = b1.prvi.x;
	b2.drugi.y = b1.prvi.y - 1;
	return b2;
}

blok desni_jedan(blok b1)
{
	blok b2;
	b2.prvi.x = b1.prvi.x;
	b2.prvi.y = b1.prvi.y + 1;
	b2.drugi.x = b1.prvi.x;
	b2.drugi.y = b1.prvi.y + 2;
	return b2;
}

blok gore_jedan(blok b1)
{
	blok b2;
	b2.prvi.x = b1.prvi.x - 2;
	b2.prvi.y = b1.prvi.y;
	b2.drugi.x = b1.prvi.x - 1;
	b2.drugi.y = b1.prvi.y;
	return b2;
}

blok dole_jedan(blok b1)
{
	blok b2;
	b2.prvi.x = b1.prvi.x + 1;
	b2.prvi.y = b1.prvi.y;
	b2.drugi.x = b1.prvi.x + 2;
	b2.drugi.y = b1.prvi.y;
	return b2;
}

//dva gore-dole

blok levi_dva(blok b1)
{
	blok b2;
	b2.prvi.x = b1.prvi.x;
	b2.prvi.y = b1.prvi.y - 1;
	b2.drugi.x = b1.drugi.x;
	b2.drugi.y = b1.drugi.y - 1;
	return b2;
}

blok desni_dva(blok b1)
{
	blok b2;
	b2.prvi.x = b1.prvi.x;
	b2.prvi.y = b1.prvi.y + 1;
	b2.drugi.x = b1.drugi.x;
	b2.drugi.y = b1.drugi.y + 1;
	return b2;
}

blok gore_dva(blok b1)
{
	blok b2;
	b2.prvi.x = b1.prvi.x - 1;
	b2.prvi.y = b1.prvi.y;
	b2.drugi = b2.prvi;
	return b2;
}

blok dole_dva(blok b1)
{
	blok b2;
	b2.prvi.x = b1.drugi.x + 1;
	b2.prvi.y = b1.prvi.y;
	b2.drugi = b2.prvi;
	return b2;
}

//dva levo-desno
blok levi_tri(blok b1)
{
	blok b2;
	b2.prvi.x = b1.drugi.x;
	b2.prvi.y = b1.prvi.y - 1;
	b2.drugi = b2.prvi;
	return b2;
}

blok desni_tri(blok b1)
{
	blok b2;
	b2.prvi.x = b1.drugi.x;
	b2.prvi.y = b1.drugi.y + 1;
	b2.drugi = b2.prvi;
	return b2;
}

blok dole_tri(blok b1)
{
	blok b2;
	b2.prvi.x = b1.prvi.x + 1;
	b2.prvi.y = b1.prvi.y;
	b2.drugi.x = b1.drugi.x + 1;
	b2.drugi.y = b1.drugi.y;
	return b2;
}

blok gore_tri(blok b1)
{
	blok b2;
	b2.prvi.x = b1.prvi.x - 1;
	b2.prvi.y = b1.prvi.y;
	b2.drugi.x = b1.drugi.x - 1;
	b2.drugi.y = b1.drugi.y;
	return b2;
}

int IsBlok(graph* g, blok b1)
{
	if (g->board[b1.prvi.x][b1.prvi.y] == '-') return 0;
	if (g->board[b1.drugi.x][b1.drugi.y] == '-') return 0;
	return 1;
}

void add_vertex(graph* g, blok b1, blok b2, int s) //1-levo    2-desno   3-gore    4-dole
{
	int k1 = -1; int k2 = -1;
	for (int i = 0; i < g->n; i++)
	{
		if (EqBlok(g->cvorovi[i],b1))
		{
			k1 = i;
		}
		if (EqBlok(g->cvorovi[i], b2))
		{
			k2 = i;
		}
	}
	if (k1 != -1 && k2 != -1)
	{
		if (!IsBlok(g, b1)) return;
		if (!IsBlok(g, b2)) return;
		g->a[k1][k2] = s;
	}
}

void delete_vertex(graph* g, blok b1, blok b2)
{
	int k1 = -1; int k2 = -1;
	for (int i = 0; i < g->n; i++)
	{
		if (EqBlok(g->cvorovi[i], b1))
		{
			k1 = i;
		}
		if (EqBlok(g->cvorovi[i], b2))
		{
			k2 = i;
		}
	}
	if (k1 != -1 && k2 != -1)
	{
		if (!IsBlok(g, b1)) return;
		if (!IsBlok(g, b2)) return;
		g->a[k1][k2] = 0;
	}
}

void fill_graph(graph* g)
{
	FILE* f;
	f = fopen("board.txt", "r");
	char c; int x = 0; int y = 0;
	while ((c = fgetc(f)) != EOF)
	{
		if (c == '\n')
		{
			x++;
			y = 0;
		}
		else y++;
	}
	x++;
	g->x = x; g->y = y;
	fclose(f);
	g->board = malloc(x * sizeof(char*));
	for (int i = 0; i < x; i++)
	{
		g->board[i] = malloc(y * sizeof(char));
	}
	x = 0; y = 0;
	f = fopen("board.txt", "r");
	while ((c = fgetc(f)) != EOF)
	{
		if (c == '\n')
		{
			x++;
			y = 0;
		}
		else
		{
			g->board[x][y] = c;
			if (g->board[x][y] != 'S' && g->board[x][y] != 'T' && g->board[x][y] != 'o') g->board[x][y] = '-';
			if (g->board[x][y] == 'S')
			{
				g->pocblok.prvi.x= x;
				g->pocblok.prvi.y= y;
				g->pocblok.drugi = g->pocblok.prvi;
				g->trenblok.prvi.x = x;
				g->trenblok.prvi.y = y;
				g->trenblok.drugi = g->trenblok.prvi;
			}
			if (g->board[x][y] == 'T')
			{
				g->krajblok.prvi.x = x;
				g->krajblok.prvi.y = y;
				g->krajblok.drugi = g->krajblok.prvi;
			}
			y++;
		}
	}
	fclose(f);
}

void update_graph(graph* g)
{
	for (int i = 0; i < g->n; i++)
	{
		for (int j = 0; j < g->n; j++)
		{
			g->a[i][j] = 0;
		}
	}
	for (int i = 0; i < g->n; i++)
	{
		blok b1 = g->cvorovi[i];
		if (b1.prvi.x == b1.drugi.x && b1.prvi.y == b1.drugi.y)
		{
			blok b2 = levi_jedan(b1);
			add_vertex(g, b1, b2, 1);
			b2 = desni_jedan(b1);
			add_vertex(g, b1, b2, 2);
			b2 = dole_jedan(b1);
			add_vertex(g, b1, b2, 4);
			b2 = gore_jedan(b1);
			add_vertex(g, b1, b2, 3);
		}
		else if (b1.prvi.x == b1.drugi.x)
		{
			blok b2 = levi_tri(b1);
			add_vertex(g, b1, b2, 1);
			b2 = desni_tri(b1);
			add_vertex(g, b1, b2, 2);
			b2 = dole_tri(b1);
			add_vertex(g, b1, b2, 4);
			b2 = gore_tri(b1);
			add_vertex(g, b1, b2, 3);
		}
		else if (b1.prvi.y = b1.drugi.y)
		{
			blok b2 = levi_dva(b1);
			add_vertex(g, b1, b2, 1);
			b2 = desni_dva(b1);
			add_vertex(g, b1, b2, 2);
			b2 = dole_dva(b1);
			add_vertex(g, b1, b2, 4);
			b2 = gore_dva(b1);
			add_vertex(g, b1, b2, 3);
		}
	}
}

void entagle_graph(graph* g)
{
	int br = 0;
	for (int i = 0; i < g->x; i++)
	{
		for (int j = 0; j < g->y; j++)
		{
			koor k1;
			k1.x = i;
			k1.y = j;
			koor k2;
			k2.x = i;
			k2.y = j;
			blok b;
			b.prvi = k1;
			b.drugi = k2;
			g->cvorovi[br] = b;
			br++;
			if (i > 0)
			{
				k1.x = i - 1;
				k1.y = j;
				b.prvi = k1;
				b.drugi = k2;
				g->cvorovi[br] = b;
				br++;
			}
			if (j > 0)
			{
				k1.x = i;
				k1.y = j - 1;
				b.prvi = k1;
				b.drugi = k2;
				g->cvorovi[br] = b;
				br++;
			}
			if (i < g->x - 1)
			{
				k1.x = i + 1;
				k1.y = j;
				b.prvi = k1;
				b.drugi = k2;
				g->cvorovi[br] = b;
				br++;
			}
			if (j < g->y - 1)
			{
				k1.x = i;
				k1.y = j + 1;
				b.prvi = k1;
				b.drugi = k2;
				g->cvorovi[br] = b;
				br++;
			}
		}
	}
	update_graph(g);
}

graph* init_graph()
{
	graph* g = malloc(sizeof(graph));
	fill_graph(g);
	int n = 12 + 4 * 2 * (g->x - 2) + 4 * 2 * (g->y - 2) + 5 * (g->x - 2)*(g->y - 2);
	g->a = malloc(n * sizeof(int));
	g->n = n;
	for (int i = 0; i < n; i++)
	{
		g->a[i] = malloc(n * sizeof(int));
		for (int j = 0; j < n; j++)
		{
			g->a[i][j] = 0;
		}
	}
	g->cvorovi = malloc(n * sizeof(blok));
	entagle_graph(g);
	return g;
}

void write_graph(graph* g)
{
	for (int i = 0; i < g->x; i++)
	{
		for (int j = 0; j < g->y; j++)
		{
			if (g->trenblok.prvi.x == i && g->trenblok.prvi.y == j) printf("x");
			else if (g->trenblok.drugi.x == i && g->trenblok.drugi.y == j) printf("x");
			else printf("%c", g->board[i][j]);
		}
		printf("\n");
	}
	//printf("%d %d\n", g->pocx, g->pocy);
	//printf("%d %d\n", g->krajx, g->krajy);
}

void write_given_graph(graph *g, blok b)
{
	for (int i = 0; i < g->x; i++)
	{
		for (int j = 0; j < g->y; j++)
		{
			if (b.prvi.x == i && b.prvi.y == j) printf("x");
			else if (b.drugi.x == i && b.drugi.y == j) printf("x");
			else printf("%c", g->board[i][j]);
		}
		printf("\n");
	}
}

int move(graph* g, char c)
{
	int pom;
	if (c == 'a') pom = 1;
	else if (c == 'd') pom = 2;
	else if (c == 'w') pom = 3;
	else if (c == 's') pom = 4;
	else return 0;
	int k1;
	blok b1 = g->trenblok;
	for (int i = 0; i < g->n; i++)
	{
		if (EqBlok(b1, g->cvorovi[i]))
		{
			k1 = i;
			break;
		}
	}
	int k2=-1;
	for (int i = 0; i < g->n; i++)
	{
		if (g->a[k1][i] == pom) k2 = i;
	}
	if (k2 != -1)
	{
		g->trenblok = g->cvorovi[k2];
		if (EqBlok(g->trenblok, g->krajblok)) return 1;
		else return 0;
	}
	else return -1;
}

void add_tile(graph* g, int x, int y)
{
	if (x < g->x && y < g->y)
	{
		g->board[x][y] = 'o';
		update_graph(g);
	}
}

void remove_tile(graph* g, int x, int y)
{
	if (x < g->x && y < g->y)
	{
		g->board[x][y] = '-';
		update_graph(g);
	}
}

graph* delete_graph(graph* g)
{
	free(g->cvorovi);
	for (int i = 0; i < g->x; i++)
	{
		free(g->board[i]);
		free(g->a[i]);
	}
	free(g->board);
	free(g->a);
	g = NULL;
	return g;
}

void write_map(graph* g)
{
	FILE* fw;
	fw = fopen("board.txt", "w");
	for (int i = 0; i < g->x; i++)
	{
		for (int j = 0; j < g->y; j++)
		{
			fprintf(fw, "%c", g->board[i][j]);
		}
		if (i != g->x - 1) fprintf(fw, "\n");
	}
	fclose(fw);
}

int find_index(graph* g, blok b1)
{
	int k1 = -1;
	for (int i = 0; i < g->n; i++)
	{
		if (EqBlok(b1, g->cvorovi[i]))
		{
			k1 = i; break;
		}
	}
	return k1;
}

void visit(graph* g, int* vis, blok b1)
{
	int k1 = find_index(g, b1);
	if (k1 != -1) vis[k1] = 1;
}

int is_visited(graph* g, int* vis, blok b1)
{
	int k1 = find_index(g, b1);
	if (k1 != -1) return vis[k1];
	else return 0;
}

void find_solution(graph* g)
{
	int *vis;
	vis = malloc(g->n * sizeof(int));
	for (int i = 0; i < g->n; i++)
	{
		vis[i] = 0;
	}
	blok *queue;
	blok* path;
	queue = malloc(g->n * sizeof(int));
	path = malloc(g->n * sizeof(int));
	queue[0] = g->trenblok;
	path[0] = g->trenblok;
	visit(g, vis, g->trenblok);
	int br = 1; int zavrs = 0;
	for (int i = 0; i < br; i++)
	{
		int v = find_index(g, queue[i]);
		for (int j = 0; j < g->n; j++)
		{
			if (g->a[v][j] > 0 && !is_visited(g, vis, g->cvorovi[j]))
			{
				visit(g, vis, g->cvorovi[j]);
				queue[br] = g->cvorovi[j];
				br++;
				path[j] = g->cvorovi[v];
				if (EqBlok(g->cvorovi[j], g->krajblok))
				{
					zavrs = 1;
					break;
				}
			}
		}
		if (zavrs) break;
	}
	if (!is_visited(g, vis, g->krajblok))
	{
		printf("Nema resenja!");
		Sleep(500);
	}
	else
	{
		int brk = 0;
		blok *sol = malloc(g->n * sizeof(blok));
		blok b = g->krajblok;
		sol[brk] = b; brk++;
		while (!EqBlok(b, g->trenblok))
		{
			b = path[find_index(g, b)];
			printf("%d %d %d %d\n", b.prvi.x, b.prvi.y, b.drugi.x, b.drugi.y);
			sol[brk] = b; brk++;
		}
		for (int i = brk-1; i>=0; i--)
		{
			system("cls");
			write_given_graph(g, sol[i]);
			Sleep(1000);
		}
	}
}


void play_game()
{
	write_graph(g);
	printf("Pritisnite h za pomoc, ESC za izlaz\n");
	g->trenblok = g->pocblok;
	while (1)
	{
		if (_kbhit())
		{
			char c = _getch();
			if (c == 'h')
			{
				find_solution(g);
			}
			if (c == 27) return;
			int p = move(g, c);
			system("cls");
			if (p == 1)
			{
				printf("Pobedili ste!\n");
				break;
			}
			else if (p == -1)
			{
				printf("Izgubili ste!\n");
				break;
			}
			else
			{
				write_graph(g);
				printf("Pritisnite h za pomoc, ESC za izlaz\n");
			}
			continue;
		}
	}
	Sleep(1000);
	return;
}

void menu()
{
	printf("BLOXORZ\n");
	printf("1.Inicijalizuj tablu\n");
	printf("2.Igraj igru\n");
	printf("3.Dodaj plocu\n");
	printf("4.Oduzmi plocu\n");
	printf("5.Vidi tablu\n");
	printf("6.Izbrisi tablu\n");
	printf("7.Upisi tablu\n");
	printf("8.Izadji\n");
	while (1)
	{
		if (_kbhit())
		{
			char c = _getch();
			if (c == '1')
			{
				g = init_graph();
				system("cls");
				printf("Tabla inicijalizovana!\n");
				Sleep(1000);
			}
			if (c == '2')
			{
				if (g == NULL)
				{
					system("cls");
					printf("Prvo inicijalizujte tablu!\n");
					Sleep(1000);
				}
				else play_game();
			}
			if (c == '3')
			{
				if (g == NULL)
				{
					system("cls");
					printf("Prvo inicijalizujte tablu!\n");
					Sleep(1000);
				}
				else
				{
					system("cls");
					printf("Unesite koordinate koje zelite da izmenite!\n"); int x; int y;
					scanf("%d %d", &x, &y);
					add_tile(g, x, y);
					Sleep(500);
				}
			}
			if (c == '4')
			{
				if (g == NULL)
				{
					system("cls");
					printf("Prvo inicijalizujte tablu!\n");
					Sleep(1000);
				}
				else
				{
					system("cls");
					printf("Unesite koordinate koje zelite da izmenite!\n"); int x; int y;
					scanf("%d %d", &x, &y);
					remove_tile(g, x, y);
					Sleep(500);
				}
			}
			if (c == '5')
			{
				if (g == NULL)
				{
					system("cls");
					printf("Prvo inicijalizujte tablu!\n");
					Sleep(1000);
				}
				else
				{
					system("cls");
					write_graph(g);
					Sleep(2000);
				}
			}
			if (c == '6')
			{
				if (g == NULL)
				{
					system("cls");
					printf("Prvo inicijalizujte tablu!\n");
					Sleep(1000);
				}
				else
				{
					system("cls");
					g = delete_graph(g);
					printf("Tabla izbrisana!\n");
					Sleep(1000);
				}
			}
			if (c == '7')
			{
				if (g == NULL)
				{
					system("cls");
					printf("Prvo inicijalizujte tablu!\n");
					Sleep(1000);
				}
				else
				{
					system("cls");
					write_map(g);
					printf("Tabla upisana!\n");
					Sleep(1000);
				}
			}
			if (c == '8') exit(0);
			system("cls");
			printf("BLOXORZ\n");
			printf("1.Inicijalizuj tablu\n");
			printf("2.Igraj igru\n");
			printf("3.Dodaj plocu\n");
			printf("4.Oduzmi plocu\n");
			printf("5.Vidi tablu\n");
			printf("6.Izbrisi tablu\n");
			printf("7.Upisi tablu\n");
			printf("8.Izadji\n");
		}
	}
}

void main()
{
	menu();
	system("PAUSE");
}
