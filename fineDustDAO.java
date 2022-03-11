package fineDust;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.Statement;
import java.util.ArrayList;


public class fineDustDAO {
	Connection conn;
	Statement stmt;
	PreparedStatement ps;
	ResultSet rs;
	String url = "jdbc:oracle:thin:@localhost:1521:orcl";
	ResultSetMetaData rsmd;

	
	public ResultSet getRS(String sql) {
		try {
			String url = "jdbc:oracle:thin:@localhost:1521:orcl";
			String id = "c##hr1";
			String password = "1234";
			Class.forName("oracle.jdbc.OracleDriver");
			conn = DriverManager.getConnection(url, id, password);
			ps=conn.prepareStatement(sql);
			//ps.setInt(1, getNext()-(pagenumber-1)*10);
			rs=ps.executeQuery();
			
		} catch (Exception e) {
			// TODO: handle exception
		}
		return rs;
	}
	
	
	public FineDust getbbs(int numberbbs) {
		String sqlget="select * from finedustbbs where numberbbs=?";
		try {
			String url = "jdbc:oracle:thin:@localhost:1521:orcl";
			String id = "c##hr1";
			String password = "1234";
			Class.forName("oracle.jdbc.OracleDriver");
			conn = DriverManager.getConnection(url, id, password);
			ps=conn.prepareStatement(sqlget);
			ps.setInt(1, numberbbs);
			rs=ps.executeQuery();
			FineDust fine= new FineDust();
			if(rs.next()){
				fine.setDay(rs.getString(1));
				fine.setNumberbbs(rs.getInt(2));
				fine.setFineDust(rs.getString(3));
				fine.setTemperature(rs.getString(4));
				fine.setHumidity(rs.getString(5));
				fine.setImagekm(rs.getString(6));
				fine.setImagedb(rs.getString(7));
				return fine;
				
			}
			System.out.println(rs.getString(1));
			
		} catch (Exception e) {
			// TODO: handle exception
		}
		return null;	
	}
	public boolean nextPage(int pageNumber) {
		String SQL="select * from fine_dust where numberbbs <?";
		try {
			PreparedStatement pstmt= conn.prepareStatement(SQL);
			pstmt.setInt(1, getNext()-(pageNumber-1)*10); 
			rs=pstmt.executeQuery(); 
			if(rs.next()) {
				return true;
			}
		} catch (Exception e) {
			// TODO: handle exception
		}
		
		return false;	
		
	}
	public int getNext() {
		String SQL="select numberbbs from fine_dust order by numberbbs desk";
		try {
			PreparedStatement pstmt=conn.prepareStatement(SQL);
			rs=pstmt.executeQuery();
			if(rs.next()) {
				return rs.getInt(1)+1;
			}
			return 1;
		} catch (Exception e) {
			// TODO: handle exception
		}
		return -1;
	}
	
	
	
	public ArrayList<FineDust> getlist(int pageNumber){
		String SQL="select ³¯Â¥ from fine_dust";
		ArrayList<FineDust> list= new ArrayList<FineDust>();
		try {
			PreparedStatement pstmt= conn.prepareStatement(SQL);
			pstmt.setInt(1, getNext()-(pageNumber-1)*10); 
			rs=pstmt.executeQuery();
			while(rs.next()) {
				FineDust fine= new FineDust();
				fine.setDay(rs.getString(1));
				fine.setNumberbbs(rs.getInt(2));
				fine.setFineDust(rs.getString(3));
				fine.setTemperature(rs.getString(5));
				fine.setHumidity(rs.getString(4));
				
				
				list.add(fine);
				
				
			}
		} catch (Exception e) {
			// TODO: handle exception
		}
		
		return list;		
		
	}
	
	

 
}
