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
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.Properties;

import org.apache.xmlrpc.XmlRpcException;
import org.apache.xmlrpc.client.XmlRpcClient;
import org.apache.xmlrpc.client.XmlRpcClientConfigImpl;

public class erfclient {

	public static void main(String[] args) {
		
		if (args.length>0)
		{
			System.out.println("ERF Client version 1.0");
			LoadAppProperties("erfclient.conf");
			
			if (args[0].equals("-l"))
			{
				CheckList();
			}else if (args[0].equals("-e"))
			{
				getEvidence(args[1]);
			}
			
		}else
			usage();

	}
	
	
	static void CheckList()
	{
		
	}
	
	static void getEvidence(String filename)
	{

		String ersHost = System.getProperty("ers.host");
		String ersPort = System.getProperty("ers.port");
		
		  XmlRpcClientConfigImpl config = new XmlRpcClientConfigImpl();
		    try {
				config.setServerURL(new URL("http://"+ersHost+":"+ersPort+"/RPC2"));
			} catch (MalformedURLException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		    XmlRpcClient client = new XmlRpcClient();
		    client.setConfig(config);
		   
		    try {
		    	Object[] params = new Object[]{new String(filename)};
				String result = (String) client.execute("ERS.logacquire", params);
				
				BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
				bw.write(result);		
				bw.close();
	
				// Authenticate
				
				String hashfromfile = authenticate(filename);
	
				System.out.println("filehash");
				System.out.println(hashfromfile);
				// Get secreat and compare
				String hashfromDB = (String) client.execute("ERS.retrievesecret", params);
				
				System.out.println("DBhash");
				System.out.println(hashfromDB);
				
				if(hashfromfile.equals(hashfromDB))
				{
					System.out.println("PASS!!!!");
				}else
					System.out.println("FAIL");
				
				
		    } catch (XmlRpcException | IOException e) {
				// TODO Auto-generated catch blockd
				e.printStackTrace();
			}
	}
	
	static String authenticate(String filename)
	{
		String seed = "";
		  
		try (BufferedReader br = new BufferedReader(new FileReader(new File(filename)))) {
		    String line;
		  
		    while ((line = br.readLine()) != null) {
		       // process the line.
		    	
		    	System.out.println("authening : "+line);
		    	seed = sha256(line+seed);
		    	System.out.println("result : "+seed);
		    }
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return seed;
	}
	
	public static String sha256(String base) {
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
	static void usage()
	{
		System.out.println("========================================== ERF Client ============================================");
		System.out.println(" -l          : To list oned and sched log files in /var/log/one and status in DB");
		System.out.println(" -e FILENAME : To retrieve evidence file, file is saved on disk only if it pass ERF verification");
		System.out.println("==================================================================================================");
	}
	
	
  protected static void LoadAppProperties(String strPropertiesFile)
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
