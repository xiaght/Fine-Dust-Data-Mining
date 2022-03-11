<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ page import="fineDust.fineDustDAO"%>
<%@ page import="fineDust.FineDust"%>
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
		int numberbbs = 1;
	if (request.getParameter("numberbbs") != null) {
		numberbbs = Integer.parseInt(request.getParameter("numberbbs"));
	}
	String sqlget="select * from finedustbbs where numberbbs=?";
	FineDust fine = new fineDustDAO().getbbs(numberbbs);
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
						<th colspan="3"
							style="background-color: #eeeeee; text-align: center;">미세먼지량</th>
					</tr>
				</thead>
				<tbody>
					<tr><td style="width: 20%;">글 번호</td>
						<td colspan="2"><%out.print(fine.getNumberbbs());%></td>
					</tr>
					<tr>
						<td style="width: 20%;">날자</td>
						<td colspan="2"><%out.print(fine.getDay());%></td>
					</tr>
					<tr>
						<td style="width: 20%;">미세먼지량</td>
						<td colspan="2"><%out.print(fine.getFineDust());%></td>
					</tr>
					<tr>
						<td style="width: 20%;">기온</td>
						<td colspan="2"><%out.print(fine.getTemperature());%></td>
					</tr>
					<tr>
						<td style="width: 20%;">습도</td>
						<td colspan="2"><%out.print(fine.getHumidity());%></td>
					</tr>
				</tbody>
			</table>
			<% out.print("<img src='madia/k_means/"+fine.getImagekm()+".png'"+" width='300'height='150'>");%>
			<% out.print("<img src='madia/dbscan/"+fine.getImagedb()+".png'"+" width='300'height='150'>");%>
		</div>
	</div>
	<script scr="https://code.jquery.com/jquery-3.3.2min.js"></script>
	<script scr="js/bootstrap.js"></script>
</body>
</html>