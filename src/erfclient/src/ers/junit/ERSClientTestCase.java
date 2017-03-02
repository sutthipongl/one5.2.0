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
		
		//testSingleFile_DBStable
		new File("oned_20170301.log").delete();
		new File("oned_20170301_1.log").delete();
		new File("oned_20170301_2.log").delete();
		new File("oned_20170301_3.log").delete();
		new File("oned_20170301_4.log").delete();
		new File("oned_20170301_5.log").delete();
		new File("oned_20170301_6.log").delete();
		new File("oned_20170301_7.log").delete();
		
		//testDateRoll_DBStable
		new File("oned_20170302.log").delete();
		new File("oned_20170304.log").delete();
		new File("oned_20170306.log").delete();
		new File("oned_20170308.log").delete();
		new File("oned_20170310.log").delete();
		new File("oned_20170312.log").delete();
		new File("oned_20170314.log").delete();
		new File("oned_20170316.log").delete();
		
		new File("oned_20170303.log").delete();
		new File("oned_20170305.log").delete();
		new File("oned_20170307.log").delete();
		new File("oned_20170309.log").delete();
		new File("oned_20170311.log").delete();
		new File("oned_20170313.log").delete();
		new File("oned_20170315.log").delete();
		new File("oned_20170317.log").delete();
	}

	@Test
	public void testSingleFile_DBStable() {
		
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_1.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_2.log"));
		assertEquals(erfclient.ERF_Trust,  erfApp.getEvidence("e oned_20170301_3.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_4.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_5.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_6.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170301_7.log"));
		

	}
	
	@Test
	public void testDateRoll_DBStable() {
		
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170302.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170304.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170306.log"));
		assertEquals(erfclient.ERF_Trust,  erfApp.getEvidence("e oned_20170308.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170310.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170312.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170314.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170316.log"));
		
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170303.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170305.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170306.log"));
		assertEquals(erfclient.ERF_Trust,  erfApp.getEvidence("e oned_20170309.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170311.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170313.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170315.log"));
		assertEquals(erfclient.ERF_UnTrust,erfApp.getEvidence("e oned_20170317.log"));
	}

}
