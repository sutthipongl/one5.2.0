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
import java.util.Iterator;
import java.util.Properties;
import java.util.TreeSet;

import org.apache.xmlrpc.XmlRpcException;
import org.apache.xmlrpc.client.XmlRpcClient;
import org.apache.xmlrpc.client.XmlRpcClientConfigImpl;

public class erfclient {
	
	final static String ERS_VERSION = "ERS Client version 1.2 build 4 Mar 2017";
	final static String ERF_ONLINE = "========= ERF ONLINE ============";
	final static String ERF_OFFLINE = "========= ERF OFFLINE ===========";
	final public static String ERF_Trust = "Trust";
	final public static String ERF_UnTrust = "UnTrust";
	final public static String ERF_PartialTrust = "Patial Trust";
	
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
			case 'e' : System.out.println(getEvidence(cmd)); break;
			case 'q' : System.out.println("Good bye :)"); System.exit(0);
			case 'h' : help();
			default : help();	
		}		
	}
}
	

private void refreshList()
{
   filesCollection.clear();
   TreeSet<String> sortedFileList = new TreeSet<String>();
   BufferedWriter bw;
   String result = "";
   
   Object[] params = new Object[]{};
   try {
	    result = (String) xmlrpcclient.execute("ERS.listfile", params);
	   
   } catch (XmlRpcException e) {
		// TODO Auto-generated catch block
		System.out.println("ERS : Can't get file list, please check ERS server process or erfclient configuration");
   }

   try {
		bw = new BufferedWriter(new FileWriter("file.erf"));
		bw.write(result);		
		bw.close();
		
		try (BufferedReader br = new BufferedReader(new FileReader(new File("file.erf")))) {
		    String line;
		  
		    //Put all lines to TreeSet to get it sort
		    while ((line = br.readLine()) != null) {

		    	if ( (line.startsWith("oned_") || line.startsWith("sched_")) && line.endsWith(".log") )
		    	{  		
		    		sortedFileList.add(line);	    		
		    	}
		    }
		    
	    Iterator<String> iterator;
	    iterator = sortedFileList.iterator();
	     
	    int i=1;
	    
	    while (iterator.hasNext()){
	    	
	    	String logn = iterator.next();
	    
	    	filesCollection.put(String.valueOf(i),logn );
	    	System.out.println(i+":"+logn);
	    	++i;
	    }
	    
	    if(result.length()>0)
	    	System.out.println("Please use 'e' followed by number you want to retrieve. For example >e 1");
		} 
	
   } catch (IOException e) {
	// TODO Auto-generated catch block
		e.printStackTrace();
	   }	
}

private void help()
{
	System.out.println(" r - to get/refresh file list");
	System.out.println(" e <CHOICE> - get evidence");
	System.out.println(" h - help");
	System.out.println(" q - exit");
	
}
	
public String getEvidence(String cmd)
{
	if (cmd == null || cmd.length()==0)
		return "";
	
	
   	String filename = filesCollection.get(cmd.split(" ")[1]);
   	
   	if (filename == null)
   		filename = cmd.split(" ")[1];
   	
    try {
    	
    	Object[] params = new Object[]{new String(filename)};
    	
    	String result ="";
    	try{
    		result = (String) xmlrpcclient.execute("ERS.logacquire", params);
    		
    	}catch (org.apache.xmlrpc.XmlRpcException e)
		{
			System.out.println("ERS : Can't retrieve log file");
		}
		 
		
		BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
		bw.write(result);		
		bw.close();

	
		// Get hash and secret ,then compare
		String resultfromDB = " | ";
		try{
			 resultfromDB = (String) xmlrpcclient.execute("ERS.retrievesecret", params);
		}catch (org.apache.xmlrpc.XmlRpcException e)
		{
			System.out.println("ERS : Can't retrieve Secret");
		}
		
		String[] temp = resultfromDB.split("\\|");
		
		String hashfromDB = temp[0];
		String secretfromDB = temp[1];
		
		System.out.println("result from DB is "+ resultfromDB);
		System.out.println("DBhash is "+hashfromDB);
		System.out.println("DBsecret is "+secretfromDB);
		
	// Authenticate
		
		String hashfromfile = authenticate(filename,secretfromDB);
		System.out.println("filehash");
		System.out.println(hashfromfile);
		
		if(hashfromfile.equals(hashfromDB))
		{	if(isOnLine )
				return ERF_Trust;
			else
				return ERF_PartialTrust;
		}else
			;return ERF_UnTrust;
		
		
    } catch (IOException e) {
		// TODO Auto-generated catch blockd
		e.printStackTrace();
	}
    
    return "";
}
	
 private String authenticate(String filename,String init_seed)
{
	String seed = init_seed;
	String result ="OFFLINE";
    isOnLine = true;
	  
	try (BufferedReader br = new BufferedReader(new FileReader(new File(filename)))) {
	    String line;
	  
	    while ((line = br.readLine()) != null) {
	       // process the line.
	    	boolean isERFLine = false;
	    	
	    	//Found ERF Mark LINE , update flag
	    	if(line.equals(ERF_OFFLINE))
	    	{	
	    		isOnLine=false;
	    		isERFLine=true;
	    			
	    	}else if(line.equals(ERF_ONLINE))
	    	{	
	    		isOnLine=true;
	    		isERFLine=true;
	    	}
	    	
	    	//Always calculate hash if it's not ERF_MARK line
	    	if(!isERFLine)
	    	{
	    		seed = sha256(line+seed);
	    		System.out.println("authening : "+line);
	    	}
	    		
	    	// return last online log
	    	if(isOnLine)
	    		result = seed;
	    	
	    	
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
