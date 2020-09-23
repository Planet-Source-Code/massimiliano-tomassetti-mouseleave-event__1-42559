Attribute VB_Name = "Module1"
Option Explicit
Public Declare Function MouseLeave_AddWindow Lib "MouseLeave.dll" (ByVal hHwnd As Long, ByVal Func As Long) As Long
Public Declare Sub MouseLeave_Clear Lib "MouseLeave.dll" ()


Public b_txt1 As Boolean

Public Sub T1(ByVal hHwnd As Long)
    b_txt1 = False
    
    Form1.Text1.Text = "You Left Me"
End Sub
