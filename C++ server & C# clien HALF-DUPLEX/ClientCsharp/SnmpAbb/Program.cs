using System;
using System.Text;
using System.IO;
using System.IO.Pipes;
using System.Threading;

using System.Diagnostics;

void client(){

}

namespace CSPipe
{
    class Program
    {
        static void Main(string[] args)
        {
            
            NamedPipeClientStream pipe = new NamedPipeClientStream(".", "HyperPipe", PipeDirection.InOut);
            pipe.Connect();
             
            /*
            using (StreamReader rdr = new StreamReader(pipe, Encoding.Unicode))
            
            while (true){
               //System.Console.WriteLine(rdr.ReadToEnd());
                System.Console.WriteLine(rdr.ReadLine());
                Thread.Sleep(2000);
            }
            */


            
            StreamWriter sw = new StreamWriter(pipe);
           
                try
                {
                    sw.WriteLine("Test Message");
                    sw.Flush();
                    pipe.Close();
                }
                catch (Exception ex) { throw ex; }
            
        }
    }
}
           
      