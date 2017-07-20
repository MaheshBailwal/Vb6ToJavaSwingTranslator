import javax.swing.*;
 import java.awt.*;
 import java.awt.event.*;
  import javax.swing.border.*;
  import VbtoJava.VBfunction;

  class SexyForm1 extends JFrame
 {
 	 JPanel p1=new VBtoJavaForm();
 	 public SexyForm1()
 	{
	setSize(241,274);
	setTitle("Form1");
	Container cp=getContentPane();cp.add(p1);
	  addWindowListener(new WindowAdapter()
	 {public void windowClosing(WindowEvent e){System.exit(0);}});

	}
	public static void main(String s[])
	{
	SexyForm1 ff=new SexyForm1();
	ff.setLocation(137,90);
	ff.show();
	}
 }


class mypanel extends JPanel
{
	long fvalue;
	String opreation;
	Border etched;
	Border title;

	 TextField Text1;
	 JButton Command1;
	 JButton Command2;
	 JButton Command3;
	 JButton Command4;
	 JButton Command5;
	 JButton Command6;
	 JButton Command7;
	 JButton Command8;
	 JButton Command9;
	 JButton Command10;
	 JButton Command11;
	 JButton Command12;
	 JButton Command13;
	 JButton Command14;
	 JButton Command15;
	 JButton Command16;
	 TextField Text2;
	 TextField Text3;
	 TextField Text4;
	 public mypanel()
	{
	etched=BorderFactory.createEtchedBorder();
	 setLayout(null);
	 Text1 = new TextField("Text1");
	 Text1.setBounds(8,16,225,25);
	this.add(Text1);
	 Command1 = new JButton("1");
	 Command1.setBounds(8,160,41,33);
	this.add(Command1);
	 Command2 = new JButton("2");
	 Command2.setBounds(56,160,41,33);
	this.add(Command2);
	 Command3 = new JButton("3");
	 Command3.setBounds(109,160,41,33);
	this.add(Command3);
	 Command4 = new JButton("4");
	 Command4.setBounds(8,117,41,33);
	this.add(Command4);
	 Command5 = new JButton("5");
	 Command5.setBounds(56,120,41,33);
	this.add(Command5);
	 Command6 = new JButton("6");
	 Command6.setBounds(109,120,41,33);
	this.add(Command6);
	 Command7 = new JButton("7");
	 Command7.setBounds(8,72,41,33);
	this.add(Command7);
	 Command8 = new JButton("8");
	 Command8.setBounds(56,72,41,33);
	this.add(Command8);
	 Command9 = new JButton("9");
	 Command9.setBounds(109,72,41,33);
	this.add(Command9);
	 Command10 = new JButton("+");
	 Command10.setBounds(168,208,41,33);
	this.add(Command10);
	 Command11 = new JButton("-");
	 Command11.setBounds(168,160,41,33);
	this.add(Command11);
	 Command12 = new JButton("=");
	 Command12.setBounds(109,208,41,33);
	this.add(Command12);
	 Command13 = new JButton("*");
	 Command13.setBounds(168,120,41,33);
	this.add(Command13);
	 Command14 = new JButton("/");
	 Command14.setBounds(168,72,41,33);
	this.add(Command14);
	 Command15 = new JButton("0");
	 Command15.setBounds(8,208,41,33);
	this.add(Command15);
	 Command16 = new JButton(" ");
	 Command16.setBounds(56,208,41,33);
	this.add(Command16);
	 Text2 = new TextField("Text2");
	 Text2.setBounds(40,48,49,19);
	this.add(Text2);
	 Text3 = new TextField("Text3");
	 Text3.setBounds(88,48,49,19);
	this.add(Text3);
	 Text4 = new TextField("Text4");
	 Text4.setBounds(136,48,49,19);
	this.add(Text4);
	}
} class buttonaction extends mypanel implements ActionListener
	{
	 public buttonaction()
	{
	 Command1.addActionListener(this);
	 Command10.addActionListener(this);
	 Command11.addActionListener(this);
	 Command12.addActionListener(this);
	 Command13.addActionListener(this);
	 Command14.addActionListener(this);
	 Command15.addActionListener(this);
	 Command16.addActionListener(this);
	 Command2.addActionListener(this);
	 Command3.addActionListener(this);
	 Command4.addActionListener(this);
	 Command5.addActionListener(this);
	 Command6.addActionListener(this);
	 Command7.addActionListener(this);
	 Command8.addActionListener(this);
	 Command9.addActionListener(this);
	 }
	 public void actionPerformed(ActionEvent evt)
	{
	 Object source=evt.getSource();
	 if (source==Command1)
	 {
	 Text1.setText(""+(Text1.getText()+"1"));
	 }
	 if (source==Command10)
	 {
	  fvalue=VBfunction.Val(Text1.getText());opreation="PLUS";Text1.setText(""+(""));
	 }
	 if (source==Command11)
	 {
	  fvalue=VBfunction.Val(Text1.getText());opreation="MINUS";Text1.setText(""+(""));
	 }
	 if (source==Command12)
	 {
	  if ( opreation=="MINUS" )
{Text1.setText(""+(fvalue-VBfunction.Val(Text1.getText())));}if ( opreation=="PLUS" )
{Text1.setText(""+(fvalue+VBfunction.Val(Text1.getText())));}if ( opreation=="MULTIPLY" )
{Text1.setText(""+(fvalue*VBfunction.Val(Text1.getText())));}if ( opreation=="DIVIDE" )
{Text1.setText(""+(fvalue/VBfunction.Val(Text1.getText())));}
	 }
	 if (source==Command13)
	 {
	  fvalue=VBfunction.Val(Text1.getText());opreation="MULTIPLY";Text1.setText(""+(""));
	 }
	 if (source==Command14)
	 {
	  fvalue=VBfunction.Val(Text1.getText());Text2.setText(""+(Text1.getText()));Text3.setText(""+("/"));opreation="DIVIDE";Text1.setText(""+(""));
	 }
	 if (source==Command15)
	 {
	  Text1.setText(""+(Text1.getText()+"0"));
	 }
	 if (source==Command16)
	 {
	  Text1.setText(""+(""));
	 }
	 if (source==Command2)
	 {
	  Text1.setText(""+(Text1.getText()+"2"));
	 }
	 if (source==Command3)
	 {
	  Text1.setText(""+(Text1.getText()+"3"));
	 }
	 if (source==Command4)
	 {
	  Text1.setText(""+(Text1.getText()+"4"));
	 }
	 if (source==Command5)
	 {
	  Text1.setText(""+(Text1.getText()+"5"));
	 }
	 if (source==Command6)
	 {
	  Text1.setText(""+(Text1.getText()+"6"));
	 }
	 if (source==Command7)
	 {
	  Text1.setText(""+(Text1.getText()+"7"));
	 }
	 if (source==Command8)
	 {
	  Text1.setText(""+(Text1.getText()+"8"));
	 }
	 if (source==Command9)
	 {
	  SexyForm ff1=new SexyForm();
	  	ff1.setLocation(200,200);

	ff1.show();
	  Text1.setText(""+(Text1.getText()+"9"));
	 }
	 }
 }
	  class VBtoJavaForm extends buttonaction
	{ public VBtoJavaForm(){ }
	 }