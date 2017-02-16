package ers.testtool;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.security.MessageDigest;

public class ERS {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		try (BufferedReader br = new BufferedReader(new FileReader(new File("oned_20170215_1.log")))) {
		    String line;
		    String seed = "";
		    while ((line = br.readLine()) != null) {
		       // process the line.
		    	
		    	System.out.println("authening : "+line);
		    	seed = sha256(line+seed);
		    	System.out.println(seed);
		    }
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		

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
}
