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
		new File("oned_20170305.log").delete();
		new File("oned_20170305_0001.log").delete();
		new File("oned_20170305_0002.log").delete();
		new File("oned_20170305_0003.log").delete();
		new File("oned_20170305_0004.log").delete();
		new File("oned_20170305_0005.log").delete();
		new File("oned_20170305_0006.log").delete();
		new File("oned_20170305_0007.log").delete();
		
		//testDateRoll_DBStable
		new File("oned_20170306.log").delete();
		new File("oned_20170308.log").delete();
		new File("oned_20170310.log").delete();
		new File("oned_20170312.log").delete();
		new File("oned_20170314.log").delete();
		new File("oned_20170316.log").delete();
		new File("oned_20170318.log").delete();
		new File("oned_20170320.log").delete();
		
		new File("oned_20170307.log").delete();
		new File("oned_20170309.log").delete();
		new File("oned_20170311.log").delete();
		new File("oned_20170313.log").delete();
		new File("oned_20170315.log").delete();
		new File("oned_20170317.log").delete();
		new File("oned_20170319.log").delete();
		new File("oned_20170321.log").delete();
		
		new File("oned_20170322.log").delete();
		new File("oned_20170322_0001.log").delete();
		new File("oned_20170322_0002.log").delete();
		new File("oned_20170322_0003.log").delete();
		
		new File("oned_20170322_0004.log").delete();
		new File("oned_20170322_0005.log").delete();
		new File("oned_20170322_0006.log").delete();
		new File("oned_20170322_0007.log").delete();
		
		new File("oned_20170322_0008.log").delete();
		new File("oned_20170322_0009.log").delete();
		new File("oned_20170322_0010.log").delete();
		new File("oned_20170322_0011.log").delete();
		
		new File("oned_20170322_0012.log").delete();
		new File("oned_20170322_0013.log").delete();
		new File("oned_20170322_0014.log").delete();
		new File("oned_20170322_0015.log").delete();
		new File("oned_20170322_0016.log").delete();
		
		new File("oned663547601_ERFTEMP.log").delete();
		new File("oned91527697_ERFTEMP.log").delete();
	}

	@Test
	public void testSingleFile_DBStable() {
		
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170305.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170305_0001.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170305_0002.log"));
		assertEquals(erfclient.ERF_Trust,  		erfApp.getEvidence("e oned_20170305_0003.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170305_0004.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170305_0005.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170305_0006.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170305_0007.log"));	

	}
	
	@Test
	public void testDateRoll_DBStable() {
		
	
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170306.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170308.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170310.log"));
		assertEquals(erfclient.ERF_Trust,		erfApp.getEvidence("e oned_20170312.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170314.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170316.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170318.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170320.log"));
		
		
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170307.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170309.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170311.log"));
		assertEquals(erfclient.ERF_Trust,		erfApp.getEvidence("e oned_20170313.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170315.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170317.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170319.log"));
		assertEquals(erfclient.ERF_UnTrust,		erfApp.getEvidence("e oned_20170321.log"));
	}
	
	@Test
	public void testDBConnectionissue(){
		
		assertEquals(erfclient.ERF_PartialTrust,	erfApp.getEvidence("e oned_20170322.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170322_0001.log"));
		assertEquals(erfclient.ERF_PartialTrust,	erfApp.getEvidence("e oned_20170322_0002.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170322_0003.log"));
		
		assertEquals(erfclient.ERF_Trust,			erfApp.getEvidence("e oned_20170322_0004.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170322_0005.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170322_0006.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170322_0007.log"));
		
		assertEquals(erfclient.ERF_Trust,			erfApp.getEvidence("e oned_20170322_0008.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170322_0009.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170322_0010.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170322_0011.log"));
		
		assertEquals(erfclient.ERF_PartialTrust,	erfApp.getEvidence("e oned_20170322_0012.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170322_0013.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170322_0014.log"));
		assertEquals(erfclient.ERF_PartialTrust,	erfApp.getEvidence("e oned_20170322_0015.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170322_0016.log"));
		
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned663547601_ERFTEMP.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned91527697_ERFTEMP.log"));
		
	}

	
	@Test
	public void testRollDateDBConnectionissue(){
		
		assertEquals(erfclient.ERF_UnTrust,	erfApp.getEvidence("e oned_ERFTEMP_652570501.log"));
		assertEquals(erfclient.ERF_Trust,			erfApp.getEvidence("e oned_20170326.log"));
		assertEquals(erfclient.ERF_UnTrust,	erfApp.getEvidence("e oned_20170328.log"));
		assertEquals(erfclient.ERF_UnTrust,			erfApp.getEvidence("e oned_20170330.log"));
		
	
		
	}
}
