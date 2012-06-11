using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace PipeClient
{
    public partial class Form1 : Form
    {
        private Client pipeClient;

        public Form1()
        {
            InitializeComponent();
            this.pipeClient = new Client();
            this.pipeClient.MessageReceived += 
                new Client.MessageReceivedHandler(pipeClient_MessageReceived);
        }

        void pipeClient_MessageReceived(string message)
        {
            this.Invoke(new Client.MessageReceivedHandler(DisplayReceivedMessage),
                new object[] { message });
        }

        void DisplayReceivedMessage(string message)
        {
            this.tbReceived.Text += message + "\r\n";
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if (!this.pipeClient.Connected)
            {
                this.pipeClient.PipeName = this.tbPipeName.Text;
                this.pipeClient.Connect();
                this.btnStart.Enabled = false;
            }
            else
                MessageBox.Show("Already connected.");
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            this.pipeClient.SendMessage(this.tbSend.Text);
        }
    }
}