 import javax.xml.parsers.DocumentBuilder;
 import javax.xml.parsers.DocumentBuilderFactory;
 import javax.xml.parsers.ParserConfigurationException;
 import javax.xml.transform.Transformer;
 import javax.xml.transform.TransformerFactory;
 import javax.xml.transform.dom.DOMSource;
 import javax.xml.transform.stream.StreamResult;
 import org.w3c.dom.Document;
 import org.w3c.dom.Element;
 import java.io.File;
 //import java.io.StringWriter;


 public class XmlFileWriter {

	 static File file;

	 public XmlFileWriter(File file){
		 this.file = file;
	 }

  	 //public void logCustomer(String lastNmStr, String firstNmStr, String idStr, String nationStr) throws Exception{
	 public void logCustomer(String[] cusInfoStr) throws Exception{

		//�ٻ�ý�����DocumentBuilder�Ĺ���ʵ��DocumentBuilderFactory  Ȼ���õ�DocumentBuilder����
		DocumentBuilder newDocumentBuilder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
		//�ڻ�ȡһ��������ļ������ķǿ�Document����
		Document doc = newDocumentBuilder.parse(file);
		//��ͨ���ĵ������ø��ĵ�����ĸ��ڵ�
		Element root = doc.getDocumentElement();

		//����һ���µ�employee�ڵ�
		Element customer = doc.createElement("Customer");

		//����customer�ڵ�ļ����ӽڵ�
		Element lname = doc.createElement("LastName");
		Element fname = doc.createElement("FirstName");
		Element id = doc.createElement("Id");
		Element nation = doc.createElement("Nationality");

        lname.setTextContent(cusInfoStr[1]);
		fname.setTextContent(cusInfoStr[0]);
		id.setTextContent(cusInfoStr[2]);
        nation.setTextContent(cusInfoStr[3]);

		//���ӽڵ�׷�ӵ�customer
		customer.appendChild(lname);
		customer.appendChild(fname);
		customer.appendChild(id);
		customer.appendChild(nation);

		//��customer��id����ֵ
		//customer.setAttribute("id", "4");
		//��customer׷�ӵ����ڵ�
		root.appendChild(customer);

		//ע�⣺XML�ļ��Ǳ����ص��ڴ��� �޸�Ҳ�����ڴ��� ==�������Ҫ���ڴ��е�����ͬ����������
		/*
		* static TransformerFactory newInstance():��ȡ TransformerFactory ����ʵ����
		* abstract  Transformer newTransformer():����ִ�д� Source �� Result �ĸ��Ƶ��� Transformer��
		* abstract  void transform(Source xmlSource, Result outputTarget):�� XML Source ת��Ϊ Result��
		*/
		Transformer transformer = TransformerFactory.newInstance().newTransformer();
		DOMSource source = new DOMSource(doc);
		StreamResult result = new StreamResult(file);
		transformer.transform(source, result);//�� XML==>Source ת��Ϊ Result
		}
}