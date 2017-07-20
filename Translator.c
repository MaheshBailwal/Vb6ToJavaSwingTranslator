#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<stdio.h>
#include<sys\stat.h>
#include<io.h>
#include"list.h"

char *javafile;
struct mbform
{
	char name[30];
	char caption[30];
	char height[8];
	char width[8];
	char top[8];
	char left[8];
	char state[2];
};

struct mbcontrol
{
	char type[15];
	char name[20];
	char caption[20];
	char height[5];
	char width[5];
	char text[15];
	char tab[3];
	char top[5];
	char left[5];
	char list[200];
	struct mbcontrol *next;
	struct mbcontrol *child;
	short int value;
};

struct mbevent
{
	char name[20];
	char source[20];
	char code[500];
	struct mbevent *next;
};

struct Jevent
{
	char name[20];
	char source[20];
	char code[500];
	struct Jevent *next;
};

struct gvariable
{
	char declare[120];
	struct gvariable *next;
};

char  vtjText[1][3][10] = { {"Text","setText","getText"} };
char  vtjButton[1][3][10] = { {"Caption","setText","getText"} };
char  vtjEvent[1][2][30] = { {"Click","ActionPerformed"} };
char  vtjvariable[4][2][10] = { {"Integer","int"},
	{"String","String"},
	{"Long","long"},
	{"Double","double"}
};
char vbfunction[1][2][30] = { {"Val","VBfunction.Val"} };

struct mbform myform;
struct mbcontrol  *constart, *conend;
struct mbevent    *evestart, *eveend;
struct Jevent *jevestart, *jeveend;
struct gvariable *gvstart, *gvend;


char * trim(char *s)
{
	char s1[40];
	int i, j = 0, tog = 0;

	for (i = 0; i <= strlen(s); i++)
	{
		if (s[i] != ' '&& s[i] != '\n')
			tog = 1;

		if (tog == 1)
		{
			s1[j] = s[i];
			j++;
		}
	}

	strcpy(s1, strrev(s1));
	j = 0;
	tog = 0;

	for (i = 0; i <= strlen(s1); i++)
	{
		if (s1[i] != ' '&& s1[i] != '\n')
			tog = 1;

		if (tog == 1)
		{
			s[j] = s1[i];
			j++;
		}
	}

	strcpy(s, strrev(s));

	return s;
}

char * getvalue(FILE **f1, int no)
{
	FILE *f;
	char s[20], *end;
	char c;
	int i = 0;
	f = *f1;

	while (1)
	{
		c = fgetc(f);

		if (c == EOF)
			break;

		s[i] = c;

		if (c == '=')
		{
			i = 0;

			while (1)
			{
				if (c == EOF)
					break;

				c = fgetc(f);
				s[i] = c;

				if (c == '\n')
				{
					s[i] = '\0';
					strcpy(s, trim(s));

					if (no == 1)
					{
						ltoa(strtol(s, &end, 10) / 15, s, 10);
					}

					return s;
				}

				i++;
			}
		}
	}

	return s;
}

char * get_fname(FILE **f1)
{
	FILE *f;
	char c;
	char s[50];
	int i = 0;
	f = *f1;

	while (1)
	{
		c = fgetc(f);

		if (c == EOF)
			break;

		s[i] = c;

		if (c == '\n')
		{
			s[i] = '\0';
			printf("%s%", s);
			strcpy(s, trim(s));

			return s;
		}

		i++;

		if (c == '=')
			i = 0;
	}

	return s;
}

char* get_variable(FILE **f)
{
	FILE *f1;
	char c, s[30], var[100], s1[120];

	int i = 0, tog = 0, j = 0, n = 0;
	f1 = *f;

	while (1)
	{
		c = fgetc(f1);
		if (c == EOF)
			break;

		s[i] = c;

		if (tog == 0 && c != ' ' && c != '\t')
		{
			var[j] = c;
			j++;
		}

		if (c == ' ' || c == '\t')
		{
			s[i] = '\0';

			if ((strcmp(s, "As")) == 0)
			{
				tog = 1;
				var[j - 2] = '\0';
			}

			i = -1;
		}

		if (c == '\n')
		{
			s[i] = '\0';

			for (n = 0; n < 4; n++)
			{
				if ((strcmp(s, vtjvariable[n][0])) == 0)
				{
					strcpy(s, vtjvariable[n][1]);
				}
			}

			strcpy(s1, s);
			strcat(s1, " ");
			strcat(s1, var);
			strcat(s1, ";");
			return s1;
		}

		i++;
	}
}

void get_gvariables(FILE **f)
{
	char *s;
	FILE *f1;
	struct gvariable *temp;
	f1 = *f;

	temp = (struct gvariable *)malloc(sizeof(struct gvariable));

	temp->next = NULL;

	s = get_variable(&f1);

	strcpy(temp->declare, s);

	if (gvstart == NULL)
	{
		gvstart = gvend = temp;
	}
	else
	{
		gvend->next = temp;
		gvend = temp;
	}
}

void readframe(FILE **f, struct mbcontrol **temp1)
{

	FILE *f1;
	char c, s[50];
	int i = 0;
	static count = 0;
	struct mbcontrol *temp;
	struct mbcontrol *Child;
	Child = *temp1;

	temp = (struct mbcontrol *)malloc(sizeof(struct mbcontrol));
	temp->next = NULL;
	temp->child = NULL;

	f1 = *f;

	while (1)
	{
		c = fgetc(f1);

		if (c == EOF)
			break;

		s[i] = c;

		if (c == ' ' || c == '\n' || c == '\t')
		{
			s[i] = '\0';

			if ((strcmp(s, "VB.TextBox")) == 0)
			{
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "Text");
				strcpy(temp->caption, " ");
				strcpy(temp->list, " ");
			}

			if ((strcmp(s, "VB.CommandButton")) == 0)
			{
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "Button");
				strcpy(temp->text, " ");
				strcpy(temp->list, " ");
			}

			if ((strcmp(s, "VB.Frame")) == 0)
			{
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "Frame");
				strcpy(temp->text, " ");
				strcpy(temp->caption, " ");
				strcpy(temp->list, " ");
			}

			if ((strcmp(s, "VB.ComboBox")) == 0)
			{
				printf("comboBox");
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "ComboBox");
				strcpy(temp->text, " ");
			}

			if ((strcmp(s, "VB.ListBox")) == 0)
			{
				printf("ListBox");
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "ListBox");
				strcpy(temp->text, " ");
			}

			if ((strcmp(s, "VB.OptionButton")) == 0)
			{
				printf("Option Button");
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "RadioButton");
				strcpy(temp->text, " ");
			}

			if ((strcmp(s, "VB.CheckBox")) == 0)
			{
				printf("Option Button");
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "CheckBox");
				strcpy(temp->text, " ");
			}

			if ((strcmp(s, "List")) == 0)
			{
				strcpy(temp->list, readlist(get_fname(&f1)));
			}

			if ((strcmp(s, "Begin")) == 0)
			{
				readframe(&f1, &(temp->child));
			}

			if ((strcmp(s, "End")) == 0)
			{
				if ((*temp1) == NULL)
				{
					(*temp1) = temp;
				}
				else
				{
					while (Child->next != NULL)
					{
						Child = Child->next;
					}

					Child->next = temp;
				}

				return;
			}

			if ((strcmp(s, "Caption")) == 0)
			{
				printf("\n^^%s\n^^", temp->type);
				strcpy(temp->caption, getvalue(&f1, 0));
			}

			if ((strcmp(s, "Height")) == 0)
			{
				strcpy(temp->height, getvalue(&f1, 1));
			}

			if ((strcmp(s, "Width")) == 0)
			{
				strcpy(temp->width, getvalue(&f1, 1));
			}

			if ((strcmp(s, "Top")) == 0)
			{
				strcpy(temp->top, getvalue(&f1, 1));
			}

			if ((strcmp(s, "Left")) == 0)
			{
				strcpy(temp->left, getvalue(&f1, 1));
			}

			if ((strcmp(s, "Text")) == 0)
			{
				strcpy(temp->text, getvalue(&f1, 0));
			}

			if ((strcmp(s, "Value")) == 0)
			{
				printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
				getvalue(&f1, 0);
				temp->value = 1;
			}

			if ((strcmp(s, "TabIndex")) == 0)
			{
				strcpy(temp->tab, getvalue(&f1, 0));
			}

			i = -1;
		}

		i++;
	}
}

void readcontrol(FILE **f)
{
	FILE *f1;
	char c, s[50], s1[50];
	int i = 0;
	struct mbcontrol *temp;

	temp = (struct mbcontrol *)malloc(sizeof(struct mbcontrol));
	temp->next = NULL;
	temp->child = NULL;

	f1 = *f;

	while (1)
	{
		c = fgetc(f1);
		if (c == EOF)
			break;

		s[i] = c;

		if (c == ' ' || c == '\n' || c == '\t')
		{
			s[i] = '\0';

			if ((strcmp(s, "VB.TextBox")) == 0)
			{
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "Text");
				strcpy(temp->caption, " ");
			}

			if ((strcmp(s, "VB.CommandButton")) == 0)
			{
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "Button");
				strcpy(temp->text, " ");
			}

			if ((strcmp(s, "VB.Frame")) == 0)
			{
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "Frame");
				strcpy(temp->text, " ");
				strcpy(temp->caption, " ");
			}

			if ((strcmp(s, "VB.ComboBox")) == 0)
			{
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "ComboBox");
				strcpy(temp->text, " ");
			}

			if ((strcmp(s, "VB.ListBox")) == 0)
			{
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "ListBox");
				strcpy(temp->text, " ");
			}

			if ((strcmp(s, "VB.OptionButton")) == 0)
			{
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "RadioButton");
				temp->value = 0;
				strcpy(temp->text, " ");
			}

			if ((strcmp(s, "VB.CheckBox")) == 0)
			{
				strcpy(temp->name, get_fname(&f1));
				strcpy(temp->type, "CheckBox");
				temp->value = 0;
				strcpy(temp->text, " ");
			}

			if ((strcmp(s, "Begin")) == 0)
			{
				readframe(&f1, &(temp->child));
			}

			if ((strcmp(s, "End")) == 0)
			{
				if (constart == NULL)
				{
					constart = conend = temp;
				}
				else
				{
					conend->next = temp;
					conend = temp;
				}

				return;
			}

			if ((strcmp(s, "List")) == 0)
			{
				strcpy(s1, get_fname(&f1));
				printf("&%s&", trim(s1));
				strcpy(temp->list, readlist(s1));
			}

			if ((strcmp(s, "Caption")) == 0)
			{
				strcpy(temp->caption, getvalue(&f1, 0));
			}

			if ((strcmp(s, "Height")) == 0)
			{
				strcpy(temp->height, getvalue(&f1, 1));
			}

			if ((strcmp(s, "Width")) == 0)
			{
				strcpy(temp->width, getvalue(&f1, 1));
			}

			if ((strcmp(s, "Top")) == 0)
			{
				strcpy(temp->top, getvalue(&f1, 1));
			}

			if ((strcmp(s, "Left")) == 0)
			{
				strcpy(temp->left, getvalue(&f1, 1));
			}

			if ((strcmp(s, "Text")) == 0)
			{
				strcpy(temp->text, getvalue(&f1, 0));
			}

			if ((strcmp(s, "Value")) == 0)
			{
				printf("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
				getvalue(&f1, 0);
				temp->value = 1;
			}

			if ((strcmp(s, "TabIndex")) == 0)
			{
				strcpy(temp->tab, getvalue(&f1, 0));
			}

			i = -1;
		}
		i++;
	}

}

void readform(FILE **f)
{
	FILE *f1;
	char c, s[50];
	int i = 0;
	f1 = *f;

	while (1)
	{
		c = fgetc(f1);
		if (c == EOF)
			break;

		s[i] = c;

		if (c == ' ' || c == '\n' || c == '\t')
		{
			s[i] = '\0';
			if ((strcmp(s, "VB.Form")) == 0)
			{
				strcpy(myform.name, get_fname(&f1));
			}

			if ((strcmp(s, "Begin")) == 0)
			{
				readcontrol(&f1);
			}

			if ((strcmp(s, "End")) == 0)
				return;

			if ((strcmp(s, "Caption")) == 0)
			{
				strcpy(myform.caption, getvalue(&f1, 0));
			}

			if ((strcmp(s, "ClientHeight")) == 0)
			{
				strcpy(myform.height, getvalue(&f1, 1));
			}

			if ((strcmp(s, "ClientWidth")) == 0)
			{
				strcpy(myform.width, getvalue(&f1, 1));
			}

			if ((strcmp(s, "ClientTop")) == 0)
			{
				strcpy(myform.top, getvalue(&f1, 1));
			}

			if ((strcmp(s, "ClientLeft")) == 0)
			{
				strcpy(myform.left, getvalue(&f1, 1));
			}

			i = -1;
		}
		i++;
	}

}

void show()
{
	clrscr();
	printf("\n%s", myform.name);
	printf("\n%s", myform.caption);
	printf("\n%s", myform.height);
	printf("\n%s", myform.width);
	printf("\n%s", myform.top);
	printf("\n%s", myform.left);
}

char *buff;

void re_wp(struct mbcontrol *temp, char *parent)
{
	int radio_button = 0;
	while (temp != NULL)
	{
		printf("%s\n", temp->type);

		if ((strcmp(temp->type, "Text")) == 0)
		{
			strcat(buff, "\n\t ");
			strcat(buff, temp->name);
			strcat(buff, " = new TextField(");
			strcat(buff, temp->text);
			strcat(buff, ");\n\t ");
			strcat(buff, temp->name);
			strcat(buff, ".setBounds(");
			strcat(buff, temp->left);
			strcat(buff, ",");
			strcat(buff, temp->top);
			strcat(buff, ",");
			strcat(buff, temp->width);
			strcat(buff, ",");
			strcat(buff, temp->height);
			strcat(buff, ");\n\t");

			strcat(buff, parent);
			strcat(buff, ".");

			strcat(buff, "add(");
			strcat(buff, temp->name);
			strcat(buff, ");");
		}

		if ((strcmp(temp->type, "ComboBox")) == 0)
		{
			strcat(buff, "\n\t ");
			strcat(buff, temp->name);
			strcat(buff, " = new JComboBox(");
			strcat(buff, temp->list);
			strcat(buff, ");\n\t ");
			strcat(buff, temp->name);
			strcat(buff, ".setBounds(");
			strcat(buff, temp->left);
			strcat(buff, ",");
			strcat(buff, temp->top);
			strcat(buff, ",");
			strcat(buff, temp->width);
			strcat(buff, ",");
			strcat(buff, temp->height);
			strcat(buff, ");\n\t");

			strcat(buff, parent);
			strcat(buff, ".");

			strcat(buff, "add(");
			strcat(buff, temp->name);
			strcat(buff, ");");
		}

		if ((strcmp(temp->type, "ListBox")) == 0)
		{
			strcat(buff, "\n\t ");
			strcat(buff, temp->name);
			strcat(buff, " = new JList(");
			strcat(buff, temp->list);
			strcat(buff, ");\n\t ");
			strcat(buff, temp->name);
			strcat(buff, ".setBounds(");
			strcat(buff, temp->left);
			strcat(buff, ",");
			strcat(buff, temp->top);
			strcat(buff, ",");
			strcat(buff, temp->width);
			strcat(buff, ",");
			strcat(buff, temp->height);
			strcat(buff, ");\n\t");

			strcat(buff, parent);
			strcat(buff, ".");

			strcat(buff, "add(");
			strcat(buff, temp->name);
			strcat(buff, ");");
		}


		if ((strcmp(temp->type, "Button")) == 0)
		{
			strcat(buff, "\n\t ");
			strcat(buff, temp->name);
			strcat(buff, " = new JButton(");
			strcat(buff, temp->caption);
			strcat(buff, ");\n\t ");
			strcat(buff, temp->name);
			strcat(buff, ".setBounds(");
			strcat(buff, temp->left);
			strcat(buff, ",");
			strcat(buff, temp->top);
			strcat(buff, ",");
			strcat(buff, temp->width);
			strcat(buff, ",");
			strcat(buff, temp->height);
			strcat(buff, ");\n\t");

			strcat(buff, parent);
			strcat(buff, ".");

			strcat(buff, "add(");

			strcat(buff, temp->name);
			strcat(buff, ");");
		}

		if ((strcmp(temp->type, "RadioButton")) == 0)
		{
			if (radio_button == 0)
			{
				strcat(buff, "\n\t ");
				strcat(buff, "ButtonGroup ");

				strcat(buff, parent);
				strcat(buff, "_G");

				strcat(buff, "=new ButtonGroup();\n\t");
			}

			radio_button++;

			strcat(buff, "\n\t ");
			strcat(buff, temp->name);
			strcat(buff, " = new JRadioButton(");
			strcat(buff, temp->caption);
			if (temp->value == 1)
				strcat(buff, ",true);\n\t ");
			else
				strcat(buff, ",false);\n\t ");
			strcat(buff, temp->name);
			strcat(buff, ".setBounds(");
			strcat(buff, temp->left);
			strcat(buff, ",");
			strcat(buff, temp->top);
			strcat(buff, ",");
			strcat(buff, temp->width);
			strcat(buff, ",");
			strcat(buff, temp->height);
			strcat(buff, ");\n\t");
			strcat(buff, parent);

			strcat(buff, ".");
			strcat(buff, "add(");

			strcat(buff, temp->name);
			strcat(buff, ");\n\t");
			strcat(buff, parent);

			strcat(buff, "_G");
			strcat(buff, ".add(");
			strcat(buff, temp->name);
			strcat(buff, ");\n\t");
		}

		if ((strcmp(temp->type, "CheckBox")) == 0)
		{
			strcat(buff, "\n\t ");
			strcat(buff, temp->name);
			strcat(buff, " = new JCheckBox(");
			strcat(buff, temp->caption);
			if (temp->value == 1)
				strcat(buff, ",true);\n\t ");
			else
				strcat(buff, ",false);\n\t ");
			strcat(buff, temp->name);
			strcat(buff, ".setBounds(");
			strcat(buff, temp->left);
			strcat(buff, ",");
			strcat(buff, temp->top);
			strcat(buff, ",");
			strcat(buff, temp->width);
			strcat(buff, ",");
			strcat(buff, temp->height);
			strcat(buff, ");\n\t");

			strcat(buff, parent);
			strcat(buff, ".");

			strcat(buff, "add(");
			strcat(buff, temp->name);
			strcat(buff, ");\n\t");
		}

		if ((strcmp(temp->type, "Frame")) == 0)
		{
			if (strlen(temp->caption) > 1)
			{
				strcat(buff, "title=BorderFactory.createTitledBorder(etched,");
				strcat(buff, temp->caption);
				strcat(buff, ");\n\t ");
			}

			strcat(buff, "\n\t ");
			strcat(buff, temp->name);
			strcat(buff, " = new JPanel(null);\n\t");
			strcat(buff, temp->name);
			strcat(buff, ".setBounds(");
			strcat(buff, temp->left);
			strcat(buff, ",");
			strcat(buff, temp->top);
			strcat(buff, ",");
			strcat(buff, temp->width);
			strcat(buff, ",");
			strcat(buff, temp->height);
			strcat(buff, ");");

			strcat(buff, temp->name);

			if (strlen(temp->caption) > 1)
				strcat(buff, ".setBorder(title);\n\t");
			else
				strcat(buff, ".setBorder(etched);\n\t");

			strcat(buff, parent);
			strcat(buff, ".");

			strcat(buff, "add(");
			strcat(buff, temp->name);
			strcat(buff, ");");

			if (temp->child != NULL)
				re_wp(temp->child, temp->name);

		}

		temp = temp->next;
	}
}

void declare_variable(struct mbcontrol *temp)
{
	while (temp != NULL)
	{
		if ((strcmp(temp->type, "Text")) == 0)
		{
			strcat(buff, "\n\t TextField ");
			strcat(buff, temp->name);
			strcat(buff, ";");
		}

		if ((strcmp(temp->type, "Button")) == 0)
		{
			strcat(buff, "\n\t JButton ");
			strcat(buff, temp->name);
			strcat(buff, ";");
		}

		if ((strcmp(temp->type, "ComboBox")) == 0)
		{
			strcat(buff, "\n\t JComboBox ");
			strcat(buff, temp->name);
			strcat(buff, ";");
		}

		if ((strcmp(temp->type, "ListBox")) == 0)
		{
			strcat(buff, "\n\t JList ");
			strcat(buff, temp->name);
			strcat(buff, ";");
		}

		if ((strcmp(temp->type, "RadioButton")) == 0)
		{
			strcat(buff, "\n\t JRadioButton ");
			strcat(buff, temp->name);
			strcat(buff, ";");
		}

		if ((strcmp(temp->type, "CheckBox")) == 0)
		{
			strcat(buff, "\n\t JCheckBox ");
			strcat(buff, temp->name);
			strcat(buff, ";");
		}

		if ((strcmp(temp->type, "Frame")) == 0)
		{
			strcat(buff, "\n\t JPanel ");
			strcat(buff, temp->name);
			strcat(buff, ";");

			if (temp->child != NULL)
			{
				declare_variable(temp->child);
			}
		}

		temp = temp->next;
	}
}

char * write_panel()
{
	struct mbcontrol *temp;
	struct gvariable *temp1;
	temp1 = gvstart;
	buff = (char *)malloc(3000);
	strcpy(buff, "  ");

	strcat(buff, "\nclass mypanel extends JPanel\n{\n\t");

	while (temp1 != NULL)
	{
		strcat(buff, temp1->declare);
		strcat(buff, "\n\t");
		temp1 = temp1->next;
	}

	strcat(buff, "Border etched;\n\t");
	strcat(buff, "Border title;\n\t");
	declare_variable(constart);
	strcat(buff, "\n\t public mypanel()\n\t{");
	strcat(buff, "\n\t");

	strcat(buff, "etched=BorderFactory.createEtchedBorder();\n\t");
	strcat(buff, " setLayout(null);");
	re_wp(constart, "this");

	strcat(buff, "\n\t}\n}");
	puts(buff);
}

void write_java()
{
	char *java;
	int handle;
	java = (char *)malloc(1000);

	strcpy(java, "import javax.swing.*; \n import java.awt.*; \n import java.awt.event.*;\n  import javax.swing.border.*;\n  import VbtoJava.VBfunction;");

	strcat(java, "class ");

	strcat(java, myform.name);

	strcat(java, " extends JFrame \n { \n \t");
	strcat(java, " JPanel p1=new VBtoJavaForm(); \n \t");

	strcat(java, " public ");
	strcat(java, myform.name);
	strcat(java, "()\n \t{ \n\t");
	strcat(java, "setSize(");
	strcat(java, myform.width);
	strcat(java, ",");
	strcat(java, myform.height);
	strcat(java, ");\n\t");
	strcat(java, "setTitle(");
	strcat(java, myform.caption);
	strcat(java, ");");

	strcat(java, "\n\tContainer cp=getContentPane();cp.add(p1);\n\t  addWindowListener(new WindowAdapter()\n\t {public void windowClosing(WindowEvent e){System.exit(0);}});\n\t");

	strcat(java, "\n\t}\n");

	strcat(java, "\tpublic static void main(String s[])\n\t{\n\t");

	strcat(java, myform.name);

	strcat(java, " ff=new ");

	strcat(java, myform.name);
	strcat(java, "();\n\t");
	strcat(java, "ff.setLocation(");
	strcat(java, myform.left);
	strcat(java, ",");
	strcat(java, myform.top);
	strcat(java, ");\n\t");
	strcat(java, "ff.show();\n\t}\n }");

	if ((handle = open(javafile, O_CREAT | O_TRUNC | O_TEXT | O_WRONLY, S_IWRITE)) == -1)
	{
		perror("Error:");
		return;
	}

	write(handle, java, strlen(java));
	free(java);

	write_panel();
	write(handle, buff, strlen(buff));
	free(buff);
	close(handle);
}

void show_control(struct mbcontrol *temp)
{
	while (temp != NULL)
	{
		printf("\n\n%s", temp->name);
		printf("\n\n%s", temp->type);

		if ((strcmp(temp->type, "Text")) == 0)
		{
			printf("\n%s", temp->text);
		}

		if ((strcmp(temp->type, "Button")) == 0 || (strcmp(temp->type, "RadioButton")) == 0)
		{
			printf("\n%s", temp->caption);
		}

		printf("\n%s", temp->height);
		printf("\n%s", temp->width);
		printf("\n%s", temp->top);
		printf("\n%s", temp->left);
		printf("\n%s\n", temp->tab);

		if (temp->child != NULL)
			show_control(temp->child);

		temp = temp->next;
	}
}

void eventname(FILE **f, struct mbevent **temp)
{
	FILE *f1;
	struct mbevent *temp1;
	char c, s[50];
	int i = 0;

	temp1 = *temp;
	f1 = *f;

	if (temp1 == NULL)
		exit(1);
	while (1)
	{
		c = fgetc(f1);

		if (c == EOF)
			break;

		s[i] = c;

		if (c == '_')
		{
			s[i] = '\0';

			strcpy(temp1->source, trim(s));

			printf("%s", s);

			i = -1;
		}

		if (c == '(')
		{
			s[i] = '\0';
			strcpy(temp1->name, s);
			i = -1;
		}

		if (c == '\n')
		{
			return;
		}

		i++;
	}
}

void readevent(FILE **f)
{

	FILE *f1;
	char c, s[50];
	char proper[50];
	int i = 0, j = 0;

	struct mbevent *temp;
	char code[500];
	temp = (struct mbevent *)malloc(sizeof(struct mbevent));
	temp->next = NULL;

	f1 = *f;

	eventname(&f1, &temp);

	while (1)
	{
		c = fgetc(f1);
		if (c == EOF)
			break;

		s[i] = c;
		code[j] = c;

		if (c == ' ' || c == '\n' || c == '\t')
		{
			if (code[j - 1] == '\n')
				j--;
			s[i] = '\0';

			if ((strcmp(s, "Sub")) == 0)
			{
				code[j - 3] = '\0';

				strcpy(temp->code, code);

				if (evestart == NULL)
				{
					evestart = eveend = temp;
				}
				else
				{
					eveend->next = temp;
					eveend = temp;
				}

				return;
			}
			i = -1;
		}
		j++;
		i++;
	}
}

char jcode[500];/*90099009900909909009099009*/

char* get_control_type(struct mbcontrol  *temp, char *name, int no)
{
	static char type[15];

	if (no == 0)
	{
		strcpy(type, " ");
	}

	while (temp != NULL)
	{
		if ((strcmp(temp->name, name)) == 0)
		{
			strcpy(type, temp->type);
			return type;
		}

		if (temp->child != NULL)
			get_control_type(temp->child, name, ++no);

		temp = temp->next;
	}

	return type;
}

char * vbtojav_pro(char *name, char * proper, int side)
{
	char *type;
	int i;

	type = get_control_type(constart, name, 0);

	if ((strcmp("Text", type)) == 0)
	{
		for (i = 0; i < 1; i++)
		{
			if ((strcmp(proper, vtjText[i][0])) == 0)
			{
				if (side == 0)
				{
					return vtjText[i][1];
				}
				else
				{
					return vtjText[i][2];
				}
			}

		}

	}
	return type;
}

char * vbfun(char *name)
{
	int i;

	for (i = 0; i < 1; i++)
	{

		if ((strcmp(name, vbfunction[i][0])) == 0)
			return vbfunction[i][1];
	}

	return name;
}

void re_parse(char *code, int i)
{
	int j;
	int bool, k;
	static int ifon, q_on;
	char name[20];
	char proper[20];
	char *temp;
	char s[20];
	bool = j = k = 0;

	while (code[i] != '\0')
	{
		if (code[i] == '"')
		{
			if (q_on == 1)
				q_on = 0;
			else
				q_on = 1;
		}

		s[k] = code[i];
		if (code[i] == ' ')
		{

			s[k] = '\0';
			if ((strcmp(trim(s), "If")) == 0 && ifon == 0)
			{
				ifon = 1;
				j = 0;
			}

			if ((strcmp(trim(s), "Then")) == 0)
			{
				ifon = 0;
				name[j - 4] = '\0';

				strcat(jcode, name);
				strcat(jcode, ")");
			}

			k = -1;
		}

		k++;

		if (bool == 0)
			name[j] = code[i];

		else
			proper[j] = code[i];

		if (code[i] == '.')
		{
			bool = 1;
			name[j] = '\0';
			j = -1;
		}

		if (code[i] == '=')
		{
			if (bool == 0)
			{
				name[j] = '\0';

				if (ifon == 1)
					strcat(jcode, "if ( ");

				strcat(jcode, trim(name));

				if (ifon == 1)
					strcat(jcode, "==");
				else
					strcat(jcode, "=");

				i++;

				re_parse(code, i);
			}
			else
			{
				proper[j] = '\0';
				temp = vbtojav_pro(trim(name), trim(proper), 0);
				strcat(jcode, trim(name));
				strcat(jcode, ".");
				strcat(jcode, temp);

				strcat(jcode, "(\"\"+(");

				i++;

				re_parse(code, i);

				strcat(jcode, "))");

			}

			if (ifon != 1)
				strcat(jcode, ";");

			for (; code[i] != '\n'; i++)
				;

			ifon = 0;
			i = i + 1;

			re_parse(code, i);

			return;
		}

		if (code[i] == '+')
		{
			if (j <= 1)
			{
				strcat(jcode, "+");
				i++;
				re_parse(code, i);
				return;

			}
			proper[j] = '\0';

			if (bool == 0)
			{
				name[j] = '\0';

				strcat(jcode, trim(name));

				strcat(jcode, "+");
			}
			else
			{
				temp = vbtojav_pro(trim(name), trim(proper), 1);
				strcat(jcode, trim(name));

				strcat(jcode, ".");
				strcat(jcode, temp);
				strcat(jcode, "()");
				strcat(jcode, "+");
			}

			i++;
			re_parse(code, i);
			return;
		}

		if (code[i] == '-')
		{
			if (j <= 1)
			{
				strcat(jcode, "-");
				i++;
				re_parse(code, i);
				return;

			}
			proper[j] = '\0';

			if (bool == 0)
			{
				name[j] = '\0';

				strcat(jcode, trim(name));

				strcat(jcode, "-");
			}
			else
			{
				temp = vbtojav_pro(trim(name), trim(proper), 1);
				strcat(jcode, trim(name));

				strcat(jcode, ".");
				strcat(jcode, temp);
				strcat(jcode, "()");
				strcat(jcode, "-");
			}
			i++;
			re_parse(code, i);
			return;
		}

		if (code[i] == '*')
		{
			if (j <= 1)
			{
				strcat(jcode, "*");
				i++;
				re_parse(code, i);
				return;

			}
			proper[j] = '\0';

			if (bool == 0)
			{
				name[j] = '\0';
				strcat(jcode, trim(name));
				strcat(jcode, "*");
			}
			else
			{
				temp = vbtojav_pro(trim(name), trim(proper), 1);
				strcat(jcode, trim(name));
				strcat(jcode, ".");
				strcat(jcode, temp);
				strcat(jcode, "()");
				strcat(jcode, "*");
			}

			i++;
			re_parse(code, i);
			return;
		}

		if (code[i] == '/' && q_on == 0)
		{
			if (j <= 1)
			{
				strcat(jcode, "/");
				i++;
				re_parse(code, i);
			}

			proper[j] = '\0';

			if (bool == 0)
			{
				name[j] = '\0';
				strcat(jcode, trim(name));
				strcat(jcode, "/");
			}
			else
			{
				temp = vbtojav_pro(trim(name), trim(proper), 1);
				strcat(jcode, trim(name));
				strcat(jcode, ".");
				strcat(jcode, temp);
				strcat(jcode, "()");
				strcat(jcode, "/");
			}

			i++;
			re_parse(code, i);
			return;
		}

		if (code[i] == '(')
		{
			name[j] = '\0';
			strcat(jcode, trim(vbfun(trim(name))));
			strcat(jcode, "(");
			i++;
			re_parse(code, i);
			return;
		}

		if (code[i] == ')')
		{
			proper[j] = '\0';
			temp = vbtojav_pro(trim(name), trim(proper), 1);

			strcat(jcode, trim(name));
			strcat(jcode, ".");
			strcat(jcode, temp);
			strcat(jcode, "()");
			strcat(jcode, ")");

			i++;
			re_parse(code, i);

			return;
		}

		if (code[i] == '\n')
		{
			s[k] = '\0';

			if ((strcmp(trim(s), "If")) == 0)
			{
				ifon = 0;
				strcat(jcode, "}");
				strcpy(s, " ");
				k = 0;
				j = 0;
				i++;
				continue;
			}

			if (j <= 1)
			{
				i++;
				return;
			}

			if (ifon == 1)
				j -= 4;


			proper[j] = '\0';
			if (bool == 0)
				name[j] = '\0';

			temp = vbtojav_pro(trim(name), trim(proper), 1);
			strcat(jcode, trim(name));

			if ((strcmp(temp, " ")) != 0)
			{

				strcat(jcode, ".");
				strcat(jcode, temp);
				strcat(jcode, "()");
			}

			if (code[i + 1] != '\0')
			{
			}

			if (ifon == 1)
			{
				strcat(jcode, " )\n{");
			}

			return;
		}
		i++;
		j++;
	}
}

void codeconv()
{
	struct mbevent *temp;
	struct Jevent *temp1;
	int i;
	temp = evestart;

	while (temp != NULL)
	{
		re_parse(temp->code, 0);
		temp1 = (struct Jevent *) malloc(sizeof(struct Jevent));
		temp1->next = NULL;

		for (i = 0; i < 1; i++)
		{
			if ((strcmp(temp->name, vtjEvent[i][0])) == 0)
			{
				strcpy(temp1->name, vtjEvent[i][1]);
			}
		}

		strcpy(temp1->source, temp->source);
		strcpy(temp1->code, jcode);
		strcpy(jcode, " ");

		if (jevestart == NULL)
		{
			jevestart = jeveend = temp1;
		}
		else
		{
			jeveend->next = temp1;
			jeveend = temp1;
		}

		temp = temp->next;
	}
}

void write_events()
{
	struct Jevent *temp;
	int handle;
	buff = (char *)malloc(3000);

	strcpy(buff, " class buttonaction extends mypanel implements ActionListener\n\t{");
	strcat(buff, "\n\t public buttonaction()\n\t{\n\t ");

	temp = jevestart;
	while (temp != NULL)
	{
		if ((strcmp(temp->name, "ActionPerformed")) == 0)
		{
			strcat(buff, temp->source);
			strcat(buff, ".addActionListener(this);\n\t ");
		}
		temp = temp->next;
	}

	strcat(buff, "}\n\t ");
	strcat(buff, "public void actionPerformed(ActionEvent evt)\n\t{\n\t ");
	strcat(buff, "Object source=evt.getSource();\n\t ");

	temp = jevestart;

	while (temp != NULL)
	{
		if ((strcmp(temp->name, "ActionPerformed")) == 0)
		{
			strcat(buff, "if (source==");
			strcat(buff, temp->source);
			strcat(buff, ")\n\t {\n\t ");

			strcat(buff, temp->code);
			strcat(buff, "\n\t }\n\t ");
		}

		temp = temp->next;
	}

	strcat(buff, "}\n ");
	strcat(buff, "}\n\t ");

	clrscr();

	strcat(buff, " class VBtoJavaForm extends buttonaction \n\t{");
	strcat(buff, " public VBtoJavaForm(){ }\n\t }");

	if ((handle = open(javafile, O_TEXT | O_APPEND | O_WRONLY, S_IWRITE)) == -1)
	{
		perror("Error:");
		return;
	}

	write(handle, buff, strlen(buff));
	free(buff);
	close(handle);
}

void  main(int argc, char **argv)
{
	FILE *f;
	char c;
	char s[50];
	int i = 0;
	f = fopen(argv[1], "r");
	javafile = argv[2];

	clrscr();
	while (1)
	{
		c = fgetc(f);

		if (c == EOF)
			break;

		s[i] = c;

		if (c == ' ' || c == '\n')
		{
			s[i] = '\0';

			if ((strcmp(s, "Begin")) == 0)
			{
				readform(&f);
			}

			if ((strcmp(s, "Sub")) == 0)
			{
				readevent(&f);
			}

			if ((strcmp(s, "Dim")) == 0)
			{
				get_gvariables(&f);
			}

			i = -1;
		}

		i++;
	}

	clrscr();
	show_control(constart);
	getch();
	write_java();
	codeconv();

	write_events();
	getch();
}

show_event()
{
	struct Jevent *temp;
	temp = evestart;
	while (temp != NULL)
	{
		printf("\n%s\n", temp->name);
		printf("%s\n", temp->source);
		printf("%s", temp->code);
		temp = temp->next;
	}

	return 0;
}
