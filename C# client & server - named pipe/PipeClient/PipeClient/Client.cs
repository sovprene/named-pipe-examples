using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;
using System.Threading;
using System.IO;

namespace PipeClient
{
    class Client
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern SafeFileHandle CreateFile(
           String pipeName,
           uint dwDesiredAccess,
           uint dwShareMode,
           IntPtr lpSecurityAttributes,
           uint dwCreationDisposition,
           uint dwFlagsAndAttributes,
           IntPtr hTemplate);

        public const uint GENERIC_READ = (0x80000000);
        public const uint GENERIC_WRITE = (0x40000000);
        public const uint OPEN_EXISTING = 3;
        public const uint FILE_FLAG_OVERLAPPED = (0x40000000);

        public delegate void MessageReceivedHandler(string message);
        public event MessageReceivedHandler MessageReceived;

        public const int BUFFER_SIZE = 4096;

        string pipeName;
        private FileStream stream;
        private SafeFileHandle handle;
        Thread readThread;
        bool connected;

        public bool Connected
        {
            get { return this.connected; }
        }

        public string PipeName
        {
            get { return this.pipeName; }
            set { this.pipeName = value; }
        }

        /// <summary>
        /// Connects to the server
        /// </summary>
        public void Connect()
        {
            this.handle =
               CreateFile(
                  this.pipeName,
                  GENERIC_READ | GENERIC_WRITE,
                  0,
                  IntPtr.Zero,
                  OPEN_EXISTING,
                  FILE_FLAG_OVERLAPPED,
                  IntPtr.Zero);

            //could not create handle - server probably not running
            if (this.handle.IsInvalid)
                return;

            this.connected = true;

            //start listening for messages
            this.readThread = new Thread(new ThreadStart(Read));
            this.readThread.Start();
        }

        /// <summary>
        /// Reads data from the server
        /// </summary>
        public void Read()
        {
            this.stream = new FileStream(this.handle, FileAccess.ReadWrite, BUFFER_SIZE, true);
            byte[] readBuffer = new byte[BUFFER_SIZE];
            ASCIIEncoding encoder = new ASCIIEncoding();
            while (true)
            {
                int bytesRead = 0;

                try
                {
                    bytesRead = this.stream.Read(readBuffer, 0, BUFFER_SIZE);
                }
                catch
                {
                    //read error occurred
                    break;
                }

                //server has disconnected
                if (bytesRead == 0)
                    break;

                //fire message received event
                if (this.MessageReceived != null)
                    this.MessageReceived(encoder.GetString(readBuffer, 0, bytesRead));
            }

            

            //clean up resource
            this.stream.Close();
            this.handle.Close();
        }

        /// <summary>
        /// Sends a message to the server
        /// </summary>
        /// <param name="message"></param>
        public void SendMessage(string message)
        {
            ASCIIEncoding encoder = new ASCIIEncoding();
            byte[] messageBuffer = encoder.GetBytes(message);

            this.stream.Write(messageBuffer, 0, messageBuffer.Length);
            this.stream.Flush();
        }
    }
}
