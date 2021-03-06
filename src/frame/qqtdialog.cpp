#include "qqtdialog.h"
#include "qqtcore.h"
#include <QIcon>

QQtDialog::QQtDialog ( QWidget* parent ) :
    QDialog ( parent )
{
    /*
     * 阻挡父亲窗口内其他控件，除非本dialog关闭 show的功能强大起来 可以使用输入法
     */
    setWindowModality ( Qt::WindowModal );

#if 0
    setWindowFlags ( Qt::FramelessWindowHint | windowFlags() );
    setAttribute ( Qt::WA_TranslucentBackground, true );
    QPalette plt = palette();
    plt.setColor ( QPalette::Normal, QPalette::Shadow, QColor ( 255, 149, 12, 255 ) );
    plt.setColor ( QPalette::Normal, QPalette::Window, QColor ( 255, 255, 255, 255 ) );
    setPalette ( plt );
#endif


    setAttribute ( Qt::WA_DeleteOnClose, false );
    setAttribute ( Qt::WA_StyledBackground, true );
    setWindowFlags ( windowFlags() | Qt::WindowCloseButtonHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint );
    setWindowIcon ( QIcon ( "./skin/default/bk_logo.ico" ) );

    m_style = QQTCENTER;
}

QQtDialog::~QQtDialog()
{
}

void QQtDialog::setImageStyle ( QQtDialog::ImageStyle style ) { m_style = style; }

void QQtDialog::setPixmap ( const QString& pic )
{
    bool ret = mImage.load ( pic );
    /*请确认文件是否拷贝成功。*/
    //pline() << ret << mImg.isNull();
    update();
}

void QQtDialog::setPixmap ( const QPixmap& pixmap )
{
    mImage = pixmap.toImage();
    update();
}

void QQtDialog::setPixmap ( const QImage& image )
{
    mImage = image;
    update();
}

void QQtDialog::paintEvent ( QPaintEvent* event )
{
    if ( mImage.isNull() )
        return QDialog::paintEvent ( event );

    QStylePainter p ( this );

    switch ( m_style )
    {
        case QQTCENTER:
        {
            /**
             * 要达到居中的目标，QImage需要做的size判断很繁复，这里使用QIcon做一些中间转换的后续转换，可以很容易的达到绘制合理大小的pixmap的目的。
             * source: pixmap file image
             * QImage 输入、输出两侧是pixmap
             * 借助(+QIcon) QIcon 输入、输出两侧也是pixmap
             * dest: 所需要的、合理大小的pixmap
             */
            //这个也不行.???不能缩放...
#if 0
            QPixmap pix;
            pix.convertFromImage ( mImage );
            p.drawPixmap ( rect(), pix );
#endif

            //计算繁琐
#if 0
            QSize rSize = rect().size();
            QSize iSize = mImage.rect().size();
            int lX = iSize.width() <= rSize.width() ? iSize.width() : rSize.width();
            int lY = iSize.height() <= rSize.height() ? iSize.height() : rSize.height();
            QSize mSize ( lX, lY );
            p.drawItemPixmap ( rect(), Qt::AlignCenter, QPixmap::fromImage ( mImage.scaled ( mSize, Qt::KeepAspectRatio ) ) );
#endif

            //缩小图片的时候,失败了.???不是的,外部使用的时候请注意,你的widget最小大小是不是0,0.如果不是,那么你的fixedSize是不是太小了,能设置成功吗?
            //写法复杂
#if 0
            QIcon icon;
            icon.addPixmap ( QPixmap::fromImage ( mImage ), QIcon::Normal, QIcon::On );
            p.drawItemPixmap ( rect(), Qt::AlignCenter, icon.pixmap ( rect().size(), QIcon::Normal, QIcon::On ) );
#endif

            //经过验证,
            //QIcon的构造函数参数为pixmap的有bug,不能正常获得完整pixmap.
            //QIcon的构造函数参数为QString的是正常的.
            //以上结论错误,都是好的,没有bug,受到了外部widget,setMinimumSize的限定蛊惑,setFixedSize无效导致的.
            //不居中.
#if 0
            p.drawPixmap ( rect(), QIcon ( QPixmap::fromImage ( mImage ) ).pixmap ( rect().size(), QIcon::Normal, QIcon::On ) );
#endif

#if 1
            p.drawItemPixmap ( rect(), Qt::AlignCenter, QIcon ( QPixmap::fromImage ( mImage ) ).pixmap ( rect().size(),
                               QIcon::Normal, QIcon::On ) );
#endif

        }
        break;

        case QQTTILEDWIDTH:
        {
            p.drawItemPixmap ( rect(), Qt::AlignLeft | Qt::AlignTop,
                               /*.copy() 切出图片的左上部分使用*/
                               QPixmap::fromImage ( mImage.copy ( rect() )
                                                    .scaledToWidth ( rect().width() )
                                                  ) );
        }
        break;

        case QQTZOOMWIDTH:
        {
            p.drawItemPixmap ( rect(), Qt::AlignLeft | Qt::AlignTop,
                               /*不.copy() 切出图片的中间部分使用*/
                               QPixmap::fromImage ( mImage
                                                    .scaled ( rect().width(), mImage.height(), Qt::IgnoreAspectRatio )
                                                  ) );
        }
        break;

        case QQTTILEDHEIGHT:
        {
            p.drawItemPixmap ( rect(), Qt::AlignLeft | Qt::AlignTop,
                               /*.copy() 切出图片的左上部分使用*/
                               QPixmap::fromImage ( mImage.copy ( rect() )
                                                    .scaledToHeight ( rect().height() )
                                                  ) );
        }
        break;

        case QQTTILED:
        {
            p.drawItemPixmap ( rect(), Qt::AlignLeft | Qt::AlignTop,
                               /*.copy() 切出图片的左上部分使用*/
                               QPixmap::fromImage ( mImage.copy ( rect() )
                                                    .scaled ( rect().width(), rect().height(), Qt::KeepAspectRatio )
                                                  ) );
        }
        break;

        case QQTZOOMHEIGHT:
        {
            p.drawItemPixmap ( rect(), Qt::AlignLeft | Qt::AlignTop,
                               /*不.copy() 切出图片的中间部分使用*/
                               QPixmap::fromImage ( mImage
                                                    .scaled ( mImage.width(), rect().height(), Qt::IgnoreAspectRatio )
                                                  ) );
        }
        break;

        case QQTZOOM:
        {
            p.drawItemPixmap ( rect(), Qt::AlignLeft | Qt::AlignTop,
                               /*不.copy() 切出图片的中间部分使用*/
                               QPixmap::fromImage ( mImage
                                                    .scaled ( rect().width(), rect().height(), Qt::IgnoreAspectRatio )
                                                  ) );
        }
        break;

        case QQTZOOMWIDTH_KEEPASPECTRATIO:
        {
            p.drawItemPixmap ( rect(), Qt::AlignLeft | Qt::AlignTop,
                               /*不.copy() 切出图片的中间部分使用*/
                               QPixmap::fromImage ( mImage
                                                    .scaled ( rect().width(), mImage.height(), Qt::KeepAspectRatio )
                                                  ) );
        }
        break;

        case QQTZOOMHEIGHT_KEEPASPECTRATIO:
        {
            p.drawItemPixmap ( rect(), Qt::AlignLeft | Qt::AlignTop,
                               /*不.copy() 切出图片的中间部分使用*/
                               QPixmap::fromImage ( mImage
                                                    .scaled ( mImage.width(), rect().height(), Qt::KeepAspectRatio )
                                                  ) );
        }
        break;

        case QQTZOOM_KEEPASPECTRATIO:
        {
            p.drawItemPixmap ( rect(), Qt::AlignLeft | Qt::AlignTop,
                               /*不.copy() 切出图片的中间部分使用*/
                               QPixmap::fromImage ( mImage
                                                    .scaled ( rect().width(), rect().height(), Qt::KeepAspectRatio )
                                                  ) );
        }
        break;

        default:
        {

        }
        break;
    }

    return QDialog::paintEvent ( event );
}

