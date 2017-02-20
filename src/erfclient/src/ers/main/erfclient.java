package ers.main;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.HashMap;
import java.util.Properties;

import org.apache.xmlrpc.XmlRpcException;
import org.apache.xmlrpc.client.XmlRpcClient;
import org.apache.xmlrpc.client.XmlRpcClientConfigImpl;

public class erfclient {
	
	final static String ERS_VERSION = "ERS Client version 1.1 build 20 Feb 2017";
	final static String ERF_ONLINE = "========= ERF ONLINE ============";
	final static String ERF_OFFLINE = "========= ERF OFFLINE ===========";
    boolean isOnLine,autoGetFileList;
	String ersHost ;
	String ersPort ;
	XmlRpcClientConfigImpl xmlrpcconfig;
	XmlRpcClient xmlrpcclient;
	HashMap<String,String> filesCollection = new HashMap<String,String>();

public static void main(String[] args) {
		
	try{
		
		erfclient app = new erfclient();
		app.init();
		app.run();
		
	}catch(IOException e)
	{
		
	}
		

}
	
public void init()
{
	System.out.println(ERS_VERSION);
	LoadAppProperties("erfclient.conf");
	ersHost = System.getProperty("ers.host");
	ersPort = System.getProperty("ers.port");
	autoGetFileList = Boolean.valueOf(System.getProperty("ers.load_file_list_at_startup"));
	
	xmlrpcconfig = new XmlRpcClientConfigImpl();
	
	try {
		
		xmlrpcconfig.setServerURL(new URL("http://"+ersHost+":"+ersPort+"/RPC2"));
		
	} catch (MalformedURLException e1) {
		// TODO Auto-generated catch block
		e1.printStackTrace();
	}
	
	xmlrpcclient = new XmlRpcClient();
	xmlrpcclient.setConfig(xmlrpcconfig);
		
}
	
public void run() throws IOException
{
	String cmd="";
	
	BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	
	while(!cmd.trim().equalsIgnoreCase("q"))
	{
		System.out.print("ERFClient>");
		 cmd = br.readLine();
		
		 if(cmd.length()==0)
		 {
			 help();
			 continue;
		 }
			 
		switch(cmd.charAt(0)) {
			
			case 'r' : refreshList(); break;
			case 'e' : getEvidence(cmd); break;
			case 'q' : System.out.println("Good bye :)"); System.exit(0);
			
			default  : help();
		}
			

	}
}
	

private void refreshList()
{
   filesCollection.clear();
   
   Object[] params = new Object[]{};
   try {
	   String result = (String) xmlrpcclient.execute("ERS.listfile", params);

   BufferedWriter bw;

	   try {
			bw = new BufferedWriter(new FileWriter("file.erf"));
			bw.write(result);		
			bw.close();
			
			int i=1;
			try (BufferedReader br = new BufferedReader(new FileReader(new File("file.erf")))) {
			    String line;
			  
			    while ((line = br.readLine()) != null) {
	
			    	if ( (line.startsWith("oned_") || line.startsWith("sched_")) && line.endsWith(".log") )
			    	{
			    		filesCollection.put(String.valueOf(i), line);
			    		System.out.println(i+":"+line);
			    		++i;
			    	}
			    }
			    
			    System.out.println("Please use 'e' followed by number you want to retrieve. For example >e 1");
			} 
		
	   } catch (IOException e) {
		// TODO Auto-generated catch block
			e.printStackTrace();
		   }
	   
   } catch (XmlRpcException e) {
	// TODO Auto-generated catch block
	e.printStackTrace();
   }
   
	
}

private void help()
{
	System.out.println("Please press \"r\" to get/refresh file list and use \"e\" followed by choice you want");;
	
}
	
private void getEvidence(String cmd)
{
	if (cmd == null || cmd.length()==0)
		return;
	
	
   	String filename = filesCollection.get(cmd.split(" ")[1]);
    try {
    	
    	Object[] params = new Object[]{new String(filename)};
		String result = (String) xmlrpcclient.execute("ERS.logacquire", params);
		
		BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
		bw.write(result);		
		bw.close();

		// Authenticate
		
		String hashfromfile = authenticate(filename);

		System.out.println("filehash");
		System.out.println(hashfromfile);
		// Get secret and compare
		String hashfromDB = (String) xmlrpcclient.execute("ERS.retrievesecret", params);
		
		System.out.println("DBhash");
		System.out.println(hashfromDB);
		
		if(hashfromfile.equals(hashfromDB))
		{	if(isOnLine )
				System.out.println("TRUST");
			else
				System.out.println("Partial Trust");
		}else
			System.out.println("UNTRUST");
		
		
    } catch (XmlRpcException | IOException e) {
		// TODO Auto-generated catch blockd
		e.printStackTrace();
	}
}
	
 private String authenticate(String filename)
{
	String seed = "";
	String result ="";
    isOnLine = true;
	  
	try (BufferedReader br = new BufferedReader(new FileReader(new File(filename)))) {
	    String line;
	  
	    while ((line = br.readLine()) != null) {
	       // process the line.
	    	
	    	
	    	//Found ERF Mark LINE , update flag and not calculate hash
	    	if(line.equals(ERF_OFFLINE))
	    	{	
	    		isOnLine=false;
	    		continue;
	    		
	    	}else if(line.equals(ERF_ONLINE))
	    	{	
	    		isOnLine=true;
	    		continue;
	    	}
	    	
	    	//Always calculate hash if it's not ERF_MARK line
	    	seed = sha256(line+seed);
	    	
	    	// return last online log
	    	if(isOnLine)
	    		result = seed;
	    	
	    	System.out.println("authening : "+line);
	    	System.out.println("current : "+seed);
	    	System.out.println("result : "+result);
	    }
	} catch (FileNotFoundException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	} catch (IOException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}
	return result;
}
	
private String sha256(String base) {
	
    try{
        MessageDigest digest = MessageDigest.getInstance("SHA-256");
        byte[] hash = digest.digest(base.getBytes("UTF-8"));
        StringBuffer hexString = new StringBuffer();

        for (int i = 0; i < hash.length; i++) {
            String hex = Integer.toHexString(0xff & hash[i]);
            if(hex.length() == 1) hexString.append('0');
            hexString.append(hex);
        }

        return hexString.toString();
    } catch(Exception ex){
       throw new RuntimeException(ex);
	    }
}


	
  private static void LoadAppProperties(String strPropertiesFile)
    {     
        try
        {
            Properties oProperties = new Properties(System.getProperties());
            oProperties.load(new BufferedInputStream(new FileInputStream(strPropertiesFile)));
            System.setProperties(oProperties);

        }
        catch (IOException e)
        { 
            e.printStackTrace();
        }
     
    }

}
