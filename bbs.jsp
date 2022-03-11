<%@page import="java.sql.ResultSet"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ page import="fineDust.fineDustDAO"%>
<%@ page import="fineDust.FineDust"%>
<%@ page import="java.util.ArrayList"%>
<link rel="stylesheet" href="css/custom.css">
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<mata name="viewport" content="width=device-width" ,initial-scale="1">
<link rel="stylesheet" href="css/bootstrap.css">
<title>Fine Dust</title>
</head>
<body>
	<%
		int pagenumber = 1;
	if (request.getParameter("pagenumber") != null) {
		pagenumber = Integer.parseInt(request.getParameter("pagenumber"));
	}
	%>
	<nav class="navbar navbar-default">

		<div class="navbar-header">

			<button type="button" class="navbar-toggle collapsed"
				date-toggle="collapse" date-target="#bs-example-navbar-collapse-1"
				aria-expanded="false">
				<span class="icon-bar"></span> <span class="icon-bar"></span> <span
					class="icon-bar"></span>
			</button>
			<a class="navbar-brand" href="index.jsp">미세먼지 측정값</a>
		</div>
		<div class="collapse navbar-collapse"
			id="bs-example-navbar-collapse-1">
			<ul class="nav navbar-nav">
				<li><a href="index.jsp">메인</a></li>
				<li><a href="bbs.jsp">날씨</a></li>

			</ul>
		</div>
	</nav>
	<div class="container">
		<div class="row">
			<table class="table table-striped"
				style="text-align: center; border: 1px solid #dddddd">
				<thead>
					<tr>
						<th style="background-color: #eeeeee; text-align: center;">번호
						</th>
						<th style="background-color: #eeeeee; text-align: center;">날짜
						</th>
						<th style="background-color: #eeeeee; text-align: center;">미세먼지량
						</th>

					</tr>
				</thead>
				<tbody>
					<%
					String sql="select * from finedustbbs ";
						ResultSet rs = null;
					fineDustDAO fine = new fineDustDAO();
					rs = fine.getRS(sql);
					while (rs.next()) {
						out.print("<tr><td>" + rs.getInt(2) + "</td>");
						out.print("<td><a href='view.jsp?numberbbs=" + rs.getInt(2) + "'>" 
						+ rs.getString(1) + "</a></td>");
						out.print("<td>" + rs.getFloat(3) + "</td></tr>");


					}
					%>
				</tbody>
			</table>
			<%
				if (pagenumber != 1) {
			%>
			<a href='bbs.jsp? pagenumber=<%=pagenumber - 1%>'
				class="btn btn-success btn-arraw-left">이전</a>
			<%
				}
			if (fine.nextPage(pagenumber + 1)) {
			%>
			<a href='bbs.jsp? pagenumber=<%=pagenumber + 1%>'
				class="btn btn-success btn-arraw-left">다음</a>

			<%
				}
			%>
		</div>


	</div>



	<script scr="https://code.jquery.com/jquery-3.3.2min.js"></script>
	<script scr="js/bootstrap.js"></script>
</body>
</html>
