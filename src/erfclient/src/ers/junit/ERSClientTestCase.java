package ers.junit;

import static org.junit.Assert.*;

import java.io.File;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import ers.main.erfclient;

public class ERSClientTestCase {

	erfclient erfApp ;
	@Before
	public void setUp() throws Exception {
		
		erfApp = new erfclient();
		erfApp.init();
	}

	@After
	public void tearDown() throws Exception {
		
		new File("oned_20170301.log").delete();
		new File("oned_20170301_1.log").delete();
		new File("oned_20170301_2.log").delete();
		new File("oned_20170301_3.log").delete();
		new File("oned_20170301_4.log").delete();
		new File("oned_20170301_5.log").delete();
		new File("oned_20170301_6.log").delete();
		new File("oned_20170301_7.log").delete();
	}

	@Test
	public void test1() {
		
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_1.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_2.log"));
		assertEquals(erfclient.ERF_Trust,  erfApp.getEvidence("e oned_20170301_3.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_4.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_5.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_6.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_7.log"));
		

	}

}
