using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace PipeServer
{
    public partial class Form1 : Form
    {
        private Server pipeServer;

        public Form1()
        {
            InitializeComponent();

            this.pipeServer = new Server();

            this.pipeServer.MessageReceived += 
                new Server.MessageReceivedHandler(pipeServer_MessageReceived);
        }

        void pipeServer_MessageReceived(Server.Client client, string message)
        {
            this.Invoke(new Server.MessageReceivedHandler(DisplayMessageReceived),
                new object[] { client, message });   
        }

        void DisplayMessageReceived(Server.Client client, string message)
        {
            this.tbReceived.Text += message + "\r\n";
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            //start the pipe server if it's not already running
            if (!this.pipeServer.Running)
            {
                this.pipeServer.PipeName = this.tbPipeName.Text;
                this.pipeServer.Start();
                this.btnStart.Enabled = false;
            }
            else
                MessageBox.Show("Server already running.");
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            this.pipeServer.SendMessage(this.tbSend.Text);
        }
    }
}